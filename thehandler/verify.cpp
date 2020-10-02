#include <string>

#include <ram/gb_ram.h>
#include <utils/gb_typeutils.h>
#include <json.hpp>


#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/hex.hpp>

using json = nlohmann::json;

bool testRAMContent(const Address& addr, const RAM& ram, const std::string& rhs)
{
    // Interpret rhs string as a sequence of HEX series of values
    std::vector<ByteType> bytes;

    std::string byteBuf = "";
    for (auto i: rhs) {
        byteBuf += i;
        if (byteBuf.size() == 2) {
            BOOST_LOG_TRIVIAL(debug) << "Adding " << byteBuf;
            ByteType castedVal = std::stoul(byteBuf, NULL, 16);
            BOOST_LOG_TRIVIAL(debug) << "Casted buf to " << std::hex << castedVal;
            bytes.push_back( castedVal );
            byteBuf = "";
        }
    }

    for (auto i: bytes) {
        BOOST_LOG_TRIVIAL(debug) << i;
    }
    BOOST_LOG_TRIVIAL(debug) << "Testing content at RAM address " << std::hex << addr;
    Address ramPoint = addr;
    for (auto it = bytes.begin(); it != bytes.end(); ++it, ramPoint++)
    {
        BOOST_LOG_TRIVIAL(debug) << "Comparing expected byte " << std::hex << static_cast<unsigned int>(*it) << " == " << std::hex << static_cast<unsigned int>(ram.readByte(ramPoint));
        if (*it != ram.readByte(ramPoint)) {
            return false;
        }
    }
    return true;
}

void beginTestFromJSON(json &j, RAM &ram)
{
    /*
    Expected JSON format:
    {
        "membanks": [
            "0xFF": ".....sequence of hex bytes......",
            "0xAAAA": "0x00FF"
        ]
    }
    */

    for (auto it = j["membanks"].begin(); it != j["membanks"].end(); ++it)
    {
        std::vector<std::string> splitTestString;

        std::string testStringRaw = it->get<std::string>();
        boost::split(splitTestString, testStringRaw, boost::is_any_of(":"));
        if (splitTestString.size() != 2) {
            BOOST_LOG_TRIVIAL(error) << "Expected only two strings after string split ";
            return;
        }

        auto addr = static_cast<Address>(std::stoi(splitTestString[0], NULL, 16));
        auto expectedData = splitTestString[1];

        if (!testRAMContent(addr, ram, expectedData)) {
            BOOST_LOG_TRIVIAL(error) << "RAM Content check failed for test";
        }
    }
}

int main(int argc, char *argv[])
{
    boost::program_options::options_description description("Allowed parameters");
    description.add_options()
        ("verifyFromTemplate", boost::program_options::value<std::string>(), "File path for a structured test JSON")
        ("ramCore", boost::program_options::value<std::string>(), "Dumped RAM content from emulator")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), vm);
    boost::program_options::notify(vm);

    if (!vm.count("ramCore"))
    {
        BOOST_LOG_TRIVIAL(error) << "No ramCore specified";
        return -1;
    }

    // Load the RAM core
    RAM ram;
    std::ifstream tmpStrm;
    tmpStrm.open(vm["ramCore"].as<std::string>());
    tmpStrm >> ram;
    tmpStrm.close();

    BOOST_LOG_TRIVIAL(debug) << "Loaded ramcore as: " << ram;

    if (vm.count("verifyFromTemplate"))
    {
        json j;
        std::ifstream fstrm;
        fstrm.open(vm["verifyFromTemplate"].as<std::string>());
        fstrm >> j;
        fstrm.close();

        beginTestFromJSON(j, ram);
    }
}