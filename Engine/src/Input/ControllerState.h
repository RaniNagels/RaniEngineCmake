#pragma once
#include <cstdint>

namespace REC
{
	// wrapper class for the XINPUT_STATE so no XInput.h needs to be included into headers
    struct ControllerState
    {
        uint32_t packetNumber{};
        uint16_t buttons{};
        uint8_t leftTrigger{};
        uint8_t rightTrigger{};
        int16_t thumbLX{};
        int16_t thumbLY{};
        int16_t thumbRX{};
        int16_t thumbRY{};
    };
}