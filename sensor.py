import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import CONF_ID, ICON_EMPTY, UNIT_EMPTY

DEPENDENCIES = ["uart"]

JSY_MK_194T_decoder_ns = cg.esphome_ns.namespace("JSY_MK_194T_decoder")
JSYMK194TDecoder = JSY_MK_194T_decoder_ns.class_(
    "JSYMK194TDecoder", cg.PollingComponent, uart.UARTDevice
)

uart_ns = cg.esphome_ns.namespace("uart")
UARTComponent = uart_ns.class_("UARTComponent")

CONF_UART = "uart_id"
CONF_SENSOR1 = "active_power_1"
CONF_SENSOR2 = "active_power_2"
CONF_SENSOR3 = "positive_energy_1"
CONF_SENSOR4 = "positive_energy_2"
CONF_SENSOR5 = "negative_energy_1"
CONF_SENSOR6 = "negative_energy_2"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(JSYMK194TDecoder),
        cv.Optional(CONF_SENSOR1): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=0
        ),
        cv.Optional(CONF_SENSOR2): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=0
        ),
        cv.Optional(CONF_SENSOR3): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=3
        ),
        cv.Optional(CONF_SENSOR4): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=3
        ),
        cv.Optional(CONF_SENSOR5): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=3
        ),
        cv.Optional(CONF_SENSOR6): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=3
        ),
		cv.Required(CONF_UART): cv.use_id(UARTComponent),
    }
).extend(cv.polling_component_schema("60s")).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if sensor_1_config := config.get(CONF_SENSOR1):
        sens = await sensor.new_sensor(sensor_1_config)
        cg.add(var.set_active_power_1(sens))

    if sensor_2_config := config.get(CONF_SENSOR2):
        sens = await sensor.new_sensor(sensor_2_config)
        cg.add(var.set_active_power_2(sens))

    if sensor_3_config := config.get(CONF_SENSOR3):
        sens = await sensor.new_sensor(sensor_3_config)
        cg.add(var.set_positive_energy_1(sens))

    if sensor_4_config := config.get(CONF_SENSOR4):
        sens = await sensor.new_sensor(sensor_4_config)
        cg.add(var.set_positive_energy_2(sens))

    if sensor_5_config := config.get(CONF_SENSOR5):
        sens = await sensor.new_sensor(sensor_5_config)
        cg.add(var.set_negative_energy_1(sens))

    if sensor_6_config := config.get(CONF_SENSOR6):
        sens = await sensor.new_sensor(sensor_6_config)
        cg.add(var.set_negative_energy_2(sens))

    uart_id = await cg.get_variable(config[CONF_UART])
    cg.add(var.set_uart_id(uart_id))

    await uart.register_uart_device(var, config)
