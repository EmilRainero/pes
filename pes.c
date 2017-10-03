/** @file
 *
 * This file has been automatically generated by the WICED Smart Designer. 
 * Device configuration and functions required for the BLE device.
 *
 */

#include "bleprofile.h"
#include "bleapp.h"
#include "gpiodriver.h"
#include "string.h"
#include "stdio.h"
#include "platform.h"

// Include sensor headers
//#include "hts221_driver.h"
//#include "l3gd20_driver.h"
//#include "lis3dsh_driver.h"
//#include "lps25h_driver.h"
//#include "lsm303d_driver.h"

#include "pes_db.h"

/* add adc <meehan@parc.com> */
#include "adc.h"

/******************************************************
 *                     Constants
 ******************************************************/

#define pes_FINE_TIMER           0
#define pes_DEVICE_NAME          "pes"
#define pes_DEVICE_APPEARENCE    0
#define pes_MAIN_SERVICE_UUID    __UUID_SENSOR_SERVICE
#define pes_MAIN_CHAR_UUID       __UUID_SENSOR_SERVICE_TEMPERATURE
#define pes_MAIN_CHAR_HANDLE     HDLC_SENSOR_SERVICE_TEMPERATURE_VALUE

/******************************************************
 *                     Structures
 ******************************************************/

#pragma pack(1)
//host information for NVRAM
typedef PACKED struct
{
    //part of HOSTINFO generated by wizard
    __HOSTINFO generated;
    // ToDo: add your variables here which need to be saved in the NVRAM
}  HOSTINFO;
#pragma pack()

/******************************************************
 *               Function Prototypes
 ******************************************************/

static void pes_create(void);
static void pes_connection_up( void );
static void pes_connection_down( void );
static void pes_advertisement_stopped( void );
static void pes_smp_bond_result( LESMP_PARING_RESULT result );
static void pes_encryption_changed( HCI_EVT_HDR *evt );
static int  pes_write_handler( LEGATTDB_ENTRY_HDR *p );
static void pes_interrupt_handler( UINT8 value );

/******************************************************
 *               Variables Definitions
 ******************************************************/

const BLE_PROFILE_CFG pes_cfg =
{
    /*.fine_timer_interval            =*/ pes_FINE_TIMER, // ms
    /*.default_adv                    =*/ 4,    // HIGH_UNDIRECTED_DISCOVERABLE
    /*.button_adv_toggle              =*/ 0,    // pairing button make adv toggle (if 1) or always on (if 0)
    /*.high_undirect_adv_interval     =*/ 32,   // slots
    /*.low_undirect_adv_interval      =*/ 1024, // slots
    /*.high_undirect_adv_duration     =*/ 30,   // seconds
    /*.low_undirect_adv_duration      =*/ 300,  // seconds
    /*.high_direct_adv_interval       =*/ 0,    // seconds
    /*.low_direct_adv_interval        =*/ 0,    // seconds
    /*.high_direct_adv_duration       =*/ 0,    // seconds
    /*.low_direct_adv_duration        =*/ 0,    // seconds
    /*.local_name                     =*/ pes_DEVICE_NAME, // [LOCAL_NAME_LEN_MAX];
    /*.cod                            =*/ BIT16_TO_8(pes_DEVICE_APPEARENCE),0x00, // [COD_LEN];
    /*.ver                            =*/ "1.00",         // [VERSION_LEN];
    /*.encr_required                  =*/ 0,    //(SECURITY_ENABLED | SECURITY_REQUEST),    // data encrypted and device sends security request on every connection
    /*.disc_required                  =*/ 0,    // if 1, disconnection after confirmation
    /*.test_enable                    =*/ 1,    // TEST MODE is enabled when 1
    /*.tx_power_level                 =*/ 0x04, // dbm
    /*.con_idle_timeout               =*/ 30,   // second  0-> no timeout
    /*.powersave_timeout              =*/ 0,    // second  0-> no timeout
    /*.hdl                            =*/ {pes_MAIN_CHAR_HANDLE, 0x00, 0x00, 0x00, 0x00}, // [HANDLE_NUM_MAX];
    /*.serv                           =*/ {pes_MAIN_SERVICE_UUID, 0x00, 0x00, 0x00, 0x00},
    /*.cha                            =*/ {pes_MAIN_CHAR_UUID, 0x00, 0x00, 0x00, 0x00},
    /*.findme_locator_enable          =*/ 0,    // if 1 Find me locator is enable
    /*.findme_alert_level             =*/ 0,    // alert level of find me
    /*.client_grouptype_enable        =*/ 0,    // if 1 grouptype read can be used
    /*.linkloss_button_enable         =*/ 0,    // if 1 linkloss button is enable
    /*.pathloss_check_interval        =*/ 0,    // second
    /*.alert_interval                 =*/ 0,    // interval of alert
    /*.high_alert_num                 =*/ 0,    // number of alert for each interval
    /*.mild_alert_num                 =*/ 0,    // number of alert for each interval
    /*.status_led_enable              =*/ 1,    // if 1 status LED is enable
    /*.status_led_interval            =*/ 1,    // second
    /*.status_led_con_blink           =*/ 1,    // blink num of connection
    /*.status_led_dir_adv_blink       =*/ 0,    // blink num of dir adv
    /*.status_led_un_adv_blink        =*/ 0,    // blink num of undir adv
    /*.led_on_ms                      =*/ 200,    // led blink on duration in ms
    /*.led_off_ms                     =*/ 800,    // led blink off duration in ms
    /*.buz_on_ms                      =*/ 0,  // buzzer on duration in ms
    /*.button_power_timeout           =*/ 0,    // seconds
    /*.button_client_timeout          =*/ 0,    // seconds
    /*.button_discover_timeout        =*/ 0,    // seconds
    /*.button_filter_timeout          =*/ 0,    // seconds
#ifdef BLE_UART_LOOPBACK_TRACE
    /*.button_uart_timeout            =*/ 15,   // seconds
#endif
};

// Following structure defines UART configuration
const BLE_PROFILE_PUART_CFG pes_puart_cfg =
{
    /*.baudrate   =*/ 115200,
#ifdef GATT_DB_ENABLE_UART
    /*.txpin      =*/ GPIO_PIN_UART_TX,
    /*.rxpin      =*/ GPIO_PIN_UART_RX,
#else
    /*.txpin      =*/ PUARTDISABLE | GPIO_PIN_UART_TX,
    /*.rxpin      =*/ PUARTDISABLE | GPIO_PIN_UART_RX,
#endif
};

// NVRAM save area
HOSTINFO pes_hostinfo;
//pointer to the generated part of hostinfo assuming it is the beginning of the hostinfo
__HOSTINFO *p_hostinfo_generated = &pes_hostinfo.generated;

UINT16 	pes_connection_handle      = 0;                  // HCI handle of connection, not zero when connected
BD_ADDR pes_remote_addr            = {0, 0, 0, 0, 0, 0}; // Address of currently connected client

// ToDo: Add your static variables here

/******************************************************
 *               Function Definitions
 ******************************************************/

// Application initialization
APPLICATION_INIT()
{
    bleapp_set_cfg((UINT8 *)gatt_database,
                   gatt_database_len,
                   (void *)&pes_cfg,
                   (void *)&pes_puart_cfg,
                   (void *)&pes_gpio_cfg,
                   pes_create);
}

// Create device
void pes_create(void)
{
	extern UINT8 bleprofile_adv_num;
	extern UINT8 bleprofile_scanrsp_num;

    ble_trace0("create()");
    ble_trace0(bleprofile_p_cfg->ver);

	bleprofile_adv_num = 0x0;
	bleprofile_scanrsp_num = 0x0;

    // dump the database to debug uart.
    legattdb_dumpDb();

    bleprofile_Init(bleprofile_p_cfg);
    bleprofile_GPIOInit(bleprofile_gpio_p_cfg);

    // Initialized ROM code which will monitor the battery
    blebat_Init();

    // Read NVRAM
    bleprofile_ReadNVRAM(VS_BLE_HOST_LIST, sizeof(pes_hostinfo), (UINT8 *)&pes_hostinfo);

    // register connection up and connection down handler.
    bleprofile_regAppEvtHandler(BLECM_APP_EVT_LINK_UP, pes_connection_up);
    bleprofile_regAppEvtHandler(BLECM_APP_EVT_LINK_DOWN, pes_connection_down);
    bleprofile_regAppEvtHandler(BLECM_APP_EVT_ADV_TIMEOUT, pes_advertisement_stopped);

    // handler for Encryption changed.
    blecm_regEncryptionChangedHandler(pes_encryption_changed);

    // handler for Bond result
    lesmp_regSMPResultCb((LESMP_SINGLE_PARAM_CB) pes_smp_bond_result);

    // register to process client writes
    legattdb_regWriteHandleCb((LEGATTDB_WRITE_CB)pes_write_handler);

    // register interrupt handler
    bleprofile_regIntCb((BLEPROFILE_SINGLE_PARAM_CB) pes_interrupt_handler);

    //registers timer
    pes_reg_timer();

    // advertise first vendor specific service
    if(sizeof(pes_uuid_main_service) > 1)
    {
    	// total length should be less than 31 bytes
    	BLE_ADV_FIELD adv[3];
        BLE_ADV_FIELD scr[1];

		// flags
		adv[0].len     = 1 + 1;
		adv[0].val     = ADV_FLAGS;
		adv[0].data[0] = LE_LIMITED_DISCOVERABLE | BR_EDR_NOT_SUPPORTED;

		adv[1].len     = sizeof(pes_uuid_main_service) + 1;
		adv[1].val     = sizeof(pes_uuid_main_service) == 16 ? ADV_SERVICE_UUID128_COMP : ADV_SERVICE_UUID16_COMP;
		memcpy(adv[1].data, &pes_uuid_main_service[0], sizeof(pes_uuid_main_service));

        // Tx power level
        adv[2].len     = TX_POWER_LEN+1;
        adv[2].val     = ADV_TX_POWER_LEVEL;
        adv[2].data[0] = bleprofile_p_cfg->tx_power_level;

		// name
        scr[0].len      = strlen(bleprofile_p_cfg->local_name) + 1;
        scr[0].val      = ADV_LOCAL_NAME_COMP;
        memcpy(scr[0].data, bleprofile_p_cfg->local_name, scr[0].len - 1);

		bleprofile_GenerateADVData(adv, 3);
        bleprofile_GenerateScanRspData(scr, 1);
    }

    blecm_setTxPowerInADV(0);

    // start device advertisements.  By default Advertisements will contain flags, device name,
    // appearance and main service UUID.
    bleprofile_Discoverable(HIGH_UNDIRECTED_DISCOVERABLE, NULL);

    // ToDo: Do your initialization on app startup

    /* add adc <meehan@parc.com> */
    adc_config();
//    bleprofile_LEDBlink((UINT16)1000, (UINT16) 1000, (UINT8) 5);
}

// Connection up callback function is called on every connection establishment
void pes_connection_up(void)
{
	pes_connection_handle = (UINT16)emconinfo_getConnHandle();
	UINT8 *bda = (UINT8 *)emconninfo_getPeerPubAddr();

    // Save address of the connected device and print it out.
    memcpy(pes_remote_addr, bda, sizeof(pes_remote_addr));

    ble_trace3("connection_up: %08x%04x h=%d",
                (pes_remote_addr[5] << 24) + (pes_remote_addr[4] << 16) +
                (pes_remote_addr[3] << 8) + pes_remote_addr[2],
                (pes_remote_addr[1] << 8) + pes_remote_addr[0],
                pes_connection_handle);


    // Prepare generated code for connection - write persistent values from __HOSTINFO to GATT DB
    __on_connection_up();

	// ToDo: Write custom persistent values into GATT database using functions
	// changed_<service_name>_<char_name>() generated by smart disigner

    // If device supports a single connection, stop advertising
    bleprofile_Discoverable(NO_DISCOVERABLE, NULL);

    // If security is required for every connection following function will start bonding or
    // will setup encryption.  No indications or notifications should be sent until
    // encryption is not done.
    if (bleprofile_p_cfg->encr_required & SECURITY_REQUEST)
    {
        if (emconninfo_deviceBonded())
        {
            ble_trace0("device bonded");
        }
        else
        {
            ble_trace0("device not bonded");
            lesmp_sendSecurityRequest();
        }
    }
}

// Connection down callback
void pes_connection_down(void)
{
    ble_trace1("connection_down:handle:%d", pes_connection_handle);

    pes_connection_handle = 0;

    // If disconnection was caused by the peer, start low advertisements
    bleprofile_Discoverable(LOW_UNDIRECTED_DISCOVERABLE, NULL);

    ble_trace2("ADV start: %08x%04x",
                  (pes_remote_addr[5] << 24 ) + (pes_remote_addr[4] <<16) +
                  (pes_remote_addr[3] << 8 ) + pes_remote_addr[2],
                  (pes_remote_addr[1] << 8 ) + pes_remote_addr[0]);
}

// Callback function indicates to the application that advertising has stopped.
// restart advertisement if needed
void pes_advertisement_stopped(void)
{
    ble_trace0("ADV stop!!!!");

	// If disconnection was caused by the peer, start low advertisements
    bleprofile_Discoverable(LOW_UNDIRECTED_DISCOVERABLE, NULL);
}

// Process SMP bonding result.  If pairing is successful with the central device,
// save its BDADDR in the NVRAM and initialize associated data
void pes_smp_bond_result(LESMP_PARING_RESULT  result)
{
    ble_trace1("smp_bond_result %02x", result);

    if (result == LESMP_PAIRING_RESULT_BONDED)
    {
        // saving bd_addr in nvram
        UINT8 *bda;
        UINT8 writtenbyte;

        bda = (UINT8 *)emconninfo_getPeerPubAddr();

        // initialize persistent values in the hostinfo to add bonded peer
        pes_add_bond(bda);

        // ToDo: initialize persistent variables in HOSTINFO

        //now write hostinfo into NVRAM
        writtenbyte = bleprofile_WriteNVRAM(VS_BLE_HOST_LIST, sizeof(pes_hostinfo), (UINT8 *)&pes_hostinfo);
        ble_trace1("NVRAM write:%04x", writtenbyte);
    }
}

// Notification from the stack that encryption has been set.
void pes_encryption_changed(HCI_EVT_HDR *evt)
{
    UINT8 *bda = emconninfo_getPeerPubAddr();

    ble_trace2("encryption changed %08x%04x",
                (bda[5] << 24) + (bda[4] << 16) +
                (bda[3] << 8) + bda[2],
                (bda[1] << 8) + bda[0]);

    // ToDo: do your on-encryption-change actions here.

    // Slow down the pace of master polls to save power.  Following request asks
    // host to setup polling every 100-500 msec, with link supervision timeout 5 seconds.
    bleprofile_SendConnParamUpdateReq(80, 400, 0, 500);
}

// Process write request or command from peer device
int pes_write_handler(LEGATTDB_ENTRY_HDR *p)
{
    UINT8  writtenbyte;
    UINT16 handle   = legattdb_getHandle(p);
    int    len      = legattdb_getAttrValueLen(p);
    UINT8  *attrPtr = legattdb_getAttrValue(p);
    BOOL changed;

    ble_trace1("write_handler: handle %04x", handle);

    changed = __write_handler(handle, len, attrPtr);

    // Save update to NVRAM if it has been changed.
    if (changed)
    {
		writtenbyte = bleprofile_WriteNVRAM(VS_BLE_HOST_LIST, sizeof(pes_hostinfo), (UINT8 *)&pes_hostinfo);
		ble_trace1("NVRAM write:%04x", writtenbyte);
    }
    return 0;
}

// Three Interrupt inputs (Buttons) can be handled here.
// If the following value == 1, Button is pressed. Different than initial value.
// If the following value == 0, Button is depressed. Same as initial value.
// Button1 : value&0x01
// Button2 : (value&0x02)>>1
// Button3 : (value&0x04)>>2
void pes_interrupt_handler(UINT8 value)
{
    // ToDo: handle the interrupts here.
}

// Process indication confirmation.  if client service indication, each indication
// should be acknowledged before the next one can be sent.
void pes_indication_cfm(void)
{
}

//------ generated code

UINT16 application_read_adc_voltage_from_gpio(UINT8 gpio_number){
	return adc_readVoltage(adc_convertGPIOtoADCInput(gpio_number));
}

//status_t wiced_sense_get_humidity_temp_instantaneous_data(UINT16* humidity, INT16* temperature)
//{
//	if(HTS221_StartOneShotMeasurement() == HTS221_OK)
//	{
//		if(HTS221_Get_Measurement(humidity, temperature) == HTS221_OK)
//			return MEMS_SUCCESS;
//	}
//
//	return MEMS_ERROR;
//}

INT16 fakeTemperature = 0;
INT16 fakeHumidity = 0;

//void wiced_sense_initialize_hts221(void)
//{
//	if(HTS221_Activate() == HTS221_OK)
//		ble_trace0("HTS221_Activate Successful");
//	else
//		ble_trace0("HTS221_Activate Failed.");
//
//	if(HTS221_Set_Odr(HTS221_ODR_7HZ) == HTS221_OK)
//		ble_trace0("HTS221_Set_Odr Successful");
//	else
//		ble_trace0("HTS221_Set_Odr Failed.");
//
//	if(HTS221_Set_AvgHT(HTS221_AVGH_8, HTS221_AVGT_4) == HTS221_OK)
//		ble_trace0("HTS221_Set_AvgHT Successful");
//	else
//		ble_trace0("HTS221_Set_AvgHT failed");
//
//	if(HTS221_Set_BduMode(1) == HTS221_OK)
//		ble_trace0("HTS221_Set_BduMode Successful");
//	else
//		ble_trace0("HTS221_Set_BduMode failed");
//}

// It will be called every 1 sec
void pes_timer_1s()
{

	ble_trace0("pes_timer_1s()");

    //Todo: do you actions here every 1 second
	UINT16 value = application_read_adc_voltage_from_gpio(33);
	ble_trace1("  voltage %04x", value);

	fakeTemperature++;
	fakeHumidity++;

	value = (UINT16) fakeTemperature;

	//value = (UINT16)(status == MEMS_SUCCESS) ? 3 : 4;
	//store_in_db_sensor_service_temperature(application_read_adc_voltage_from_gpio(33), 2, TRUE, TRUE);
	store_in_db_sensor_service_temperature((UINT8 *)&fakeTemperature, 2, TRUE, TRUE);

	store_in_db_sensor_service_humidity((UINT8 *)&fakeHumidity, 2, TRUE, TRUE);

	//bleprofile_LEDBlink((UINT16)1000, (UINT16) 1000, (UINT8) 10);
}

/*
 *
 * 3D8E service � sensors
                2AE0 � temperature (16 bit unsigned)
                                temperature_history
record and return time series readings
                2BCD � humidity (16 bit unsigned)
set time � zero time
record info � every 5 seconds � shift it

blink characteristic
 *
 */


