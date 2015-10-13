//
// Created by geox on 13.10.15.
//


#include "../ftdi/ftd2xx.h"


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ftd2xx
#include <boost/test/unit_test.hpp>


struct UnitTest_ftdi {

};



BOOST_FIXTURE_TEST_SUITE(UM7LT,UnitTest_ftdi)


    BOOST_AUTO_TEST_CASE(Test_PacketSplitter_ftdi){
        BOOST_CHECK_MESSAGE(false, "Unit tests didnt finished.");
    }


BOOST_AUTO_TEST_SUITE_END()