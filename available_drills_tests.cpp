#define BOOST_TEST_MODULE availabile_drills tests
#include <boost/test/included/unit_test.hpp>

#include "units.hpp"
#include "available_drills.hpp"
namespace po = boost::program_options;

using namespace std;

BOOST_AUTO_TEST_SUITE(available_drills_tests);

AvailableDrills string_to_available_drills(const string& text) {
  stringstream ss(text);
  AvailableDrills available_drills;
  ss >> available_drills;;
  return available_drills;
}

string available_drills_to_string(const AvailableDrills& available_drills) {
  stringstream ss;
  ss << available_drills;;
  return ss.str();
}

BOOST_AUTO_TEST_CASE(parse_available_drills) {
  BOOST_CHECK_EQUAL(string_to_available_drills("4"), AvailableDrills({parse_unit<Length>("4")}));
  BOOST_CHECK_EQUAL(string_to_available_drills("25.4mm"), AvailableDrills({parse_unit<Length>("1inch")}));
  BOOST_CHECK_EQUAL(available_drills_to_string(AvailableDrills({parse_unit<Length>("1inch")})), "0.0254 m");
  BOOST_CHECK_EQUAL(available_drills_to_string(AvailableDrills({parse_unit<Length>("1")})), "1");
  BOOST_CHECK_EQUAL(available_drills_to_string(AvailableDrills({parse_unit<Length>("1inch"),
                                                                parse_unit<Length>("9")})),
    "0.0254 m, 9");
  BOOST_CHECK_EQUAL(string_to_available_drills("1mm:0.1mm"),
                    AvailableDrills({AvailableDrill(parse_unit<Length>("1mm"),
                                                    parse_unit<Length>("-0.1mm"),
                                                    parse_unit<Length>("0.1mm"))}));
  BOOST_CHECK_EQUAL(string_to_available_drills("1mm:+0.1mm:-0.2mm"),
                    AvailableDrills({AvailableDrill(parse_unit<Length>("1mm"),
                                                    parse_unit<Length>("-0.2mm"),
                                                    parse_unit<Length>("+0.1mm"))}));
  BOOST_CHECK_EQUAL(
      available_drills_to_string(
          string_to_available_drills("1inch:0.1inches")),
      "0.0254 m:-0.00254 m:+0.00254 m");

  BOOST_CHECK_THROW(string_to_available_drills(""), po::validation_error);
  BOOST_CHECK_THROW(string_to_available_drills("50.8seconds"), po::validation_error);
  BOOST_CHECK_THROW(string_to_available_drills("1mm:0.1mm:0.2mm"), po::validation_error);
  BOOST_CHECK_THROW(string_to_available_drills("1:0.1:0.2:0.3"), po::validation_error);
}


BOOST_AUTO_TEST_SUITE_END()
