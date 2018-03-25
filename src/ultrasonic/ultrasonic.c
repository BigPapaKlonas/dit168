#include <rc_usefulincludes.h>
#include <roboticscape.h>

#define BUS                             1
#define DEV_ADDRESS_ULTRASONIC_FRONT    0x71
#define DEV_ADDRESS_ULTRASONIC_REAR     0x70

// Read
#define SOFTWARE_REVISION               0x00
#define FIRST_ECHO_LOW_BYTE_REGISTER    0x03

// Write
#define COMMAND_REGISTER                0x00

// Units
#define CENTIMETER                      0x51

int main() {

    uint8_t reading_byte = 0;
    uint8_t version_byte = 0;

    // Initialize cape library
    if (rc_initialize()) {
        fprintf(stderr, "ERROR: failed to initialize rc_initialize(), are you root?\n");
        return -1;
    }

    printf("\nInitializing bus\n");
    rc_i2c_init(BUS, DEV_ADDRESS_ULTRASONIC_FRONT);

    //rc_i2c_claim_bus(BUS);

    // Triggers ultrasonic sensor to store a centimeter reading in its register
    rc_i2c_write_byte(BUS, COMMAND_REGISTER, CENTIMETER);

    // Loops while the sensor is reading
    rc_i2c_read_byte(BUS, SOFTWARE_REVISION, &version_byte);
    while (version_byte != 11) {
        rc_i2c_read_byte(BUS, SOFTWARE_REVISION, &version_byte);
    }

    // Reads the reading from sensor
    rc_i2c_read_byte(BUS, FIRST_ECHO_LOW_BYTE_REGISTER, &reading_byte);
    printf("Reading: %u cm\n", reading_byte);

    //rc_i2c_release_bus(BUS);
    rc_cleanup();
    return 0;
}
