#include <array>
#include <byte-helpers.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdio>
#include <file-table.hpp>
#include <iostream>
#include <sstream>

SCENARIO ("byte helpers can read an array of chars")
{
    GIVEN ("An empty array")
    {
        auto emptyArr = std::array<unsigned char, sizeof (size_t)> {};

        size_t sizeT;
        from_bytes (emptyArr, sizeT);

        REQUIRE (sizeT == 0);
    }
    GIVEN ("A valid array")
    {
        auto validAray = std::array<unsigned char, sizeof (size_t)> { 1, 1, 1, 1, 1, 1, 1, 1 };
        size_t sizeT;
        from_bytes (validAray, sizeT);

        REQUIRE (sizeT == 0x101010101010101);
    }
    GIVEN ("A Size_T Max array")
    {
        auto maxArray = std::array<unsigned char, sizeof (size_t)> { 255, 255, 255, 255, 255, 255, 255, 255 };
        size_t sizeT;
        from_bytes (maxArray, sizeT);
        REQUIRE (sizeT == SIZE_T_MAX);
    }

    GIVEN ("A Short Length Array")
    {
        auto shortArray = std::array<unsigned char, sizeof (short)> { 255, 255 };
        unsigned short shortResult;
        from_bytes (shortArray, shortResult);
        REQUIRE (shortResult == 0xffff);
    }
    GIVEN ("An Int Length Array")
    {
        auto intArray = std::array<unsigned char, sizeof (int)> { 255, 255, 255, 255 };
        unsigned int intResult;
        from_bytes (intArray, intResult);
        REQUIRE (intResult == 0xffffffff);
    }
}

SCENARIO ("byte helpers can write an array of chars")
{
    GIVEN ("A Size T variable of max value")
    {
        auto sizeT = SIZE_T_MAX;
        auto out = to_bytes (sizeT);

        REQUIRE (out.size() == sizeof (size_t));
        for (auto& byte : out)
        {
            REQUIRE (byte == 255);
        }
    }
}

SCENARIO ("Header handling")
{
    GIVEN ("Header can be serialized")
    {
        auto fileTable = AssetPacker::FileTable();
        fileTable.files["test"] = { 256, 0, 0 };

        auto stream = std::stringstream();
        fileTable.writeTableHeader (stream);

        fileTable.reset();

        fileTable.readTableHeader (stream);
        REQUIRE (fileTable.files.contains ("test"));
        REQUIRE (fileTable.files["test"].positionInTable == 0);
        REQUIRE (fileTable.files["test"].size == 256);
        REQUIRE (fileTable.files["test"].compressedSize == 0);
    }
}