#include "gb_ascii_screen.h"

#include <boost/log/trivial.hpp>

ASCIIScreen::ASCIIScreen(RAM &ram)
{
    BOOST_LOG_TRIVIAL(debug) << "Initializing screen objetc";
}

void ASCIIScreen::drawScreen()
{
    BOOST_LOG_TRIVIAL(debug) << "Drawing screen refresh";
}