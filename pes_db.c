/*
 * This file has been automatically generated by the WICED Smart Designer.
 * BLE device's GATT database and device configuration.
 *
 */

// pes_db.c

#include "bleprofile.h"
#include "bleapp.h"
#include "gpiodriver.h"
#include "string.h"
#include "stdio.h"
#include "platform.h"
#include "pes_db.h"

/*************************************************************************************
** GATT server definitions
*************************************************************************************/

const UINT8 gatt_database[] = // Define GATT database
{
    /* Primary Service 'Generic Access' */
    // <Name>Generic Access</Name>
    // <Uuid>1800</Uuid>
    // Service handle: HDLS_GENERIC_ACCESS
    // Service UUID: UUID_SERVICE_GAP
    PRIMARY_SERVICE_UUID16 (HDLS_GENERIC_ACCESS, UUID_SERVICE_GAP),

        /* Characteristic 'Device Name' */
        // <Name>Device Name</Name>
        // <Uuid>2A00</Uuid>
        CHARACTERISTIC_UUID16 (HDLC_GENERIC_ACCESS_DEVICE_NAME, HDLC_GENERIC_ACCESS_DEVICE_NAME_VALUE,
            UUID_CHARACTERISTIC_DEVICE_NAME, LEGATTDB_CHAR_PROP_READ,
            LEGATTDB_PERM_READABLE, 3),
            'P','E','S',

        /* Characteristic 'Appearance' */
        // <Name>Appearance</Name>
        // <Uuid>2A01</Uuid>
        CHARACTERISTIC_UUID16 (HDLC_GENERIC_ACCESS_APPEARANCE, HDLC_GENERIC_ACCESS_APPEARANCE_VALUE,
            UUID_CHARACTERISTIC_APPEARANCE, LEGATTDB_CHAR_PROP_READ,
            LEGATTDB_PERM_READABLE, 2),
            0x00,0x00,

    /* Primary Service 'Generic Attribute' */
    // <Name>Generic Attribute</Name>
    // <Uuid>1801</Uuid>
    // Service handle: HDLS_GENERIC_ATTRIBUTE
    // Service UUID: UUID_SERVICE_GATT
    PRIMARY_SERVICE_UUID16 (HDLS_GENERIC_ATTRIBUTE, UUID_SERVICE_GATT),

    /* Primary Service 'Sensor Service' */
    // <Name>Sensor Service</Name>
    // <Uuid>3D8E</Uuid>
    // Service handle: HDLS_SENSOR_SERVICE
    // Service UUID: __UUID_SENSOR_SERVICE
    PRIMARY_SERVICE_UUID16 (HDLS_SENSOR_SERVICE, __UUID_SENSOR_SERVICE),

        /* Characteristic 'Temperature' */
        // <Name>Temperature</Name>
        // <Uuid>2A6E</Uuid>
        CHARACTERISTIC_UUID16 (HDLC_SENSOR_SERVICE_TEMPERATURE, HDLC_SENSOR_SERVICE_TEMPERATURE_VALUE,
            __UUID_SENSOR_SERVICE_TEMPERATURE, LEGATTDB_CHAR_PROP_READ | LEGATTDB_CHAR_PROP_NOTIFY | LEGATTDB_CHAR_PROP_INDICATE,
            LEGATTDB_PERM_READABLE, 2),
            '0','0',

            /* Descriptor 'Client Characteristic Configuration' */
            // <Notification>true</Notification>
            // <Indication>true</Indication>
            CHAR_DESCRIPTOR_UUID16_WRITABLE (HDLD_SENSOR_SERVICE_TEMPERATURE_CLIENT_CONFIGURATION,
                UUID_DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION, LEGATTDB_PERM_READABLE | LEGATTDB_PERM_WRITE_REQ | LEGATTDB_PERM_AUTH_WRITABLE, 2),
                BIT16_TO_8(CCC_NOTIFICATION | CCC_INDICATION),

            /* Descriptor 'Characteristic Presentation Format' */
            // <PresentationFormat>
            // <Format>0x0E</Format>
            // <Exponent>0</Exponent>
            // <Unit>0x272F</Unit>
            // <NameSpace>0x01</NameSpace>
            // <Description>0x0001</Description>
            CHAR_DESCRIPTOR_UUID16 (HDLD_SENSOR_SERVICE_TEMPERATURE_PRESENTATION_FORMAT_0,
                UUID_DESCRIPTOR_CHARACTERISTIC_PRESENTATION_FORMAT, LEGATTDB_PERM_READABLE, 7),
                0x0E, 0, BIT16_TO_8(0x272F), 0x01, BIT16_TO_8(0x0001),

    /* Primary Service 'Battery Service' */
    // <Name>Battery Service</Name>
    // <Uuid>180F</Uuid>
    // Service handle: HDLS_BATTERY_SERVICE
    // Service UUID: UUID_SERVICE_BATTERY
    PRIMARY_SERVICE_UUID16 (HDLS_BATTERY_SERVICE, UUID_SERVICE_BATTERY),

        /* Characteristic 'Battery Level' */
        // <Name>Battery Level</Name>
        // <Uuid>2A19</Uuid>
        CHARACTERISTIC_UUID16 (HDLC_BATTERY_SERVICE_BATTERY_LEVEL, HDLC_BATTERY_SERVICE_BATTERY_LEVEL_VALUE,
            UUID_CHARACTERISTIC_BATTERY_LEVEL, LEGATTDB_CHAR_PROP_READ,
            LEGATTDB_PERM_READABLE, 1),
            0x00,

};
// Indication sent, waiting for ack
UINT8 pes_indication_sent = 0;
// Length of the GATT database
const UINT16 gatt_database_len = sizeof(gatt_database);

// Pointer to the bonded peer info or NULL if not bonded
__HOSTINFO *p_bonded = NULL;
// Following structure defines GPIO configuration used by the application
const BLE_PROFILE_GPIO_CFG pes_gpio_cfg =
{
    {
        GPIO_PIN_WP,                               // This need to be used to enable/disable NVRAM write protect
        GPIO_PIN_BATTERY, GPIO_PIN_LED, -1, -1, 
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 // Other GPIOs are not used
    },
    /*.gpio_flag =*/
    {
        GPIO_SETTINGS_WP,
        GPIO_SETTINGS_BATTERY, GPIO_SETTINGS_LED, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    }
};

// Main service
UINT8 pes_uuid_main_service[2] = {0x8E, 0x3D};
// Timer counter (for the trace)
UINT32 __timer_count = 0;
// Indication confirmation callback
static void __indication_cfm()
{
    pes_indication_sent = 0;
    pes_indication_cfm();
}

// Timer callback function for pes_reg_timer
void pes_timeout(UINT32 arg)
{
    ble_trace1("timeout:%d\n", __timer_count++);
    if (arg == BLEPROFILE_GENERIC_APP_TIMER)
    {
        pes_timer_1s();
    }
}
// Registers timer. Should be called from pes_create()
void pes_reg_timer()
{
    bleprofile_regTimerCb(NULL, pes_timeout);
    bleprofile_StartTimer();
}

// Finds bonded peer in the hostinfo. On exit p_bonded points on bonded peer or NULL
__HOSTINFO *__find_bonded_peer(UINT8 *bda)
{
    int i;
    p_bonded = NULL;
    if (0 == memcmp(bda, p_hostinfo_generated->bdaddr, 6))
    {
        p_bonded = p_hostinfo_generated;
    }
    return p_bonded;
}

// initializes persistent values in the hostinfo to add bonded peer
void pes_add_bond(UINT8 *bda)
{
    // Find this peer among bonded peers in the p_hostinfo_generated
    if (!__find_bonded_peer(bda))
    {
        // Peer is not found. Remember new one.
        p_bonded = p_hostinfo_generated;
        memcpy(p_bonded->bdaddr, bda, 6);
    }

    // Clear persistent generated values in the hostinfo for just bonded peer

    // Set the initial value of the client configuration descriptor for value 'Temperature'
    p_bonded->sensor_service_temperature_client_configuration = CCC_INDICATION;
}

// Prepares generated code for connection - writes persistent values from __HOSTINFO to GATT DB
void __on_connection_up()
{
    pes_indication_sent = 0;
    // Find this peer among bonded peers in the p_hostinfo_generated
    if (__find_bonded_peer(pes_remote_addr))
    {
        BLEPROFILE_DB_PDU db_pdu;

        // Write the value of the client configuration descriptor for value 'Temperature'
        db_pdu.len = 2;
        db_pdu.pdu[0] = p_bonded->sensor_service_temperature_client_configuration & 0xff;
        db_pdu.pdu[1] = (p_bonded->sensor_service_temperature_client_configuration >> 8) & 0xff;
        bleprofile_WriteHandle(HDLD_SENSOR_SERVICE_TEMPERATURE_CLIENT_CONFIGURATION, &db_pdu);
    }
}

// Updates __HOSTINFO by the value written by peer.
// Returns true if any persistent value is changed
BOOL __write_handler(UINT16 handle, int len, UINT8 *attrPtr)
{
    BOOL res = FALSE;
    if ((len == 2) && (handle == HDLD_SENSOR_SERVICE_TEMPERATURE_CLIENT_CONFIGURATION))
    {
        // find this peer among bonded peers in the p_hostinfo_generated
        if (__find_bonded_peer(pes_remote_addr))
        {
            p_bonded->sensor_service_temperature_client_configuration = attrPtr[0] + (attrPtr[1] << 8);
            ble_trace2("handle:%02x val:%04x", handle, p_bonded->sensor_service_temperature_client_configuration);
            res = TRUE;
        }
    }
    return res;
}

// It should be called when 'Device Name' is changed.
BOOL store_in_db_generic_access_device_name(UINT8* p_value, UINT8 value_len)
{
    BLEPROFILE_DB_PDU db_pdu;
    // Write value to the GATT DB
    ble_trace2("write len:%d handle:%02x", value_len, HDLC_GENERIC_ACCESS_DEVICE_NAME_VALUE);
    memcpy(&db_pdu.pdu[0], p_value, value_len);
    db_pdu.len = value_len;
    bleprofile_WriteHandle(HDLC_GENERIC_ACCESS_DEVICE_NAME_VALUE, &db_pdu);
    return TRUE;
}

// It should be called when 'Appearance' is changed.
BOOL store_in_db_generic_access_appearance(UINT8* p_value, UINT8 value_len)
{
    BLEPROFILE_DB_PDU db_pdu;
    // Write value to the GATT DB
    ble_trace2("write len:%d handle:%02x", value_len, HDLC_GENERIC_ACCESS_APPEARANCE_VALUE);
    memcpy(&db_pdu.pdu[0], p_value, value_len);
    db_pdu.len = value_len;
    bleprofile_WriteHandle(HDLC_GENERIC_ACCESS_APPEARANCE_VALUE, &db_pdu);
    return TRUE;
}

// It should be called when 'Temperature' is changed.
BOOL store_in_db_sensor_service_temperature(UINT8* p_value, UINT8 value_len, BOOL write, BOOL notify)
{
    BLEPROFILE_DB_PDU db_pdu;
    //if write is requested
    if (write)
    {
        // Write value to the GATT DB
        ble_trace2("write len:%d handle:%02x", value_len, HDLC_SENSOR_SERVICE_TEMPERATURE_VALUE);
        memcpy(&db_pdu.pdu[0], p_value, value_len);
        db_pdu.len = value_len;
        bleprofile_WriteHandle(HDLC_SENSOR_SERVICE_TEMPERATURE_VALUE, &db_pdu);
    }
    // If notification is requested
    if (notify)
    {
        if (__find_bonded_peer(pes_remote_addr))
        {
            // Exit if notify and indicate are not configured in the Client Configuration Descriptor
            if (0 == (p_bonded->sensor_service_temperature_client_configuration & (CCC_NOTIFICATION | CCC_INDICATION)))
            {
                ble_trace1("don't notify handle:%02x", HDLC_SENSOR_SERVICE_TEMPERATURE_VALUE);
                return TRUE;
            }
            // Just return FALSE if connection is down
            if (pes_connection_handle == 0)
            {
                ble_trace1("not connected handle:%02x", HDLC_SENSOR_SERVICE_TEMPERATURE_VALUE);
                return FALSE;
            }
            // Just return FALSE if we are waiting for confirmation
            if (pes_indication_sent != 0)
            {
                ble_trace1("wait confirmation handle:%02x", HDLC_SENSOR_SERVICE_TEMPERATURE_VALUE);
                return FALSE;
            }
            //if write is not requested then we did't write the value. Read it
            if (!write)
            {
                bleprofile_ReadHandle(HDLC_SENSOR_SERVICE_TEMPERATURE_VALUE, &db_pdu);
            }
            // Notify property is true. If client has registered for notification
            if (p_bonded->sensor_service_temperature_client_configuration & CCC_NOTIFICATION)
            {
                ble_trace2("notify len:%d handle:%02x", db_pdu.len, HDLC_SENSOR_SERVICE_TEMPERATURE_VALUE);
                bleprofile_sendNotification(HDLC_SENSOR_SERVICE_TEMPERATURE_VALUE, (UINT8 *)db_pdu.pdu, db_pdu.len);
            }
            // Indicate property is true. If client has registered for indication,
            // We can send only one and need to wait for ack.
            else
            {
                pes_indication_sent = 1;
                ble_trace2("indicate len:%d handle=%02x", db_pdu.len, HDLC_SENSOR_SERVICE_TEMPERATURE_VALUE);
                bleprofile_sendIndication(HDLC_SENSOR_SERVICE_TEMPERATURE_VALUE, (UINT8 *)db_pdu.pdu, db_pdu.len, __indication_cfm);
            }
        }
    }
    return TRUE;
}

// It should be called when 'Battery Level' is changed.
BOOL store_in_db_battery_service_battery_level(UINT8* p_value, UINT8 value_len)
{
    BLEPROFILE_DB_PDU db_pdu;
    // Write value to the GATT DB
    ble_trace2("write len:%d handle:%02x", value_len, HDLC_BATTERY_SERVICE_BATTERY_LEVEL_VALUE);
    memcpy(&db_pdu.pdu[0], p_value, value_len);
    db_pdu.len = value_len;
    bleprofile_WriteHandle(HDLC_BATTERY_SERVICE_BATTERY_LEVEL_VALUE, &db_pdu);
    return TRUE;
}
