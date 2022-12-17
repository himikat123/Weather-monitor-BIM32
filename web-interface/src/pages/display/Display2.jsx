import DisplayFn from "./DisplayFn2";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import SwitchInput from "../../fragments/SwitchInput";
import SelectInput from "../../fragments/SelectInput";
import NumberInput from "../../fragments/NumberInput";
import RangeInput from "../../fragments/RangeInput";
import ColorInput from "../../fragments/ColorInput";
import DisplayAutoOff from "../../fragments/DisplayAutoOff";
import DisplayBrightness from "../../fragments/DisplayBrightness";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Display extends DisplayFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        let displayTypes = [
            "--",
            `WS2812b (1 ${text.get('ledPerSegment', lang)})`,
            `WS2812b (2 ${text.get('ledsPerSegment', lang)})`,
            `WS2812b (3 ${text.get('ledsPerSegment', lang)})`
        ];

        let animations = [
            "--",
            text.get('downward', lang),
            text.get('upward', lang),
            text.get('toTheRight', lang),
            text.get('toTheLeft', lang),
            text.get('fromTheRight', lang),
            text.get('fromTheLeft', lang),
            text.get('toTheSides', lang),
            text.get('layeringFromTheRight', lang),
            text.get('layeringFromTheLeft', lang),
        ];

        let clockPoints = [
            text.get('blinkTogether', lang),
            text.get('blinkInTurn', lang),
            text.get('pointsAlwaysOn', lang),
            text.get('pointsAlwaysOff', lang)
        ];

        let displaySensors = [
            text.get('time', lang), 
            text.get('date', lang), 
            "BME280", 
            "BMP180", 
            "SHT21", 
            "DHT22", 
            "DS18B20", 
            "ESP32", 
            "Thingspeak", 
            text.get('forecast', lang),
            text.get('wirelessSensor', lang)
        ];

        let displaySensorTypesTHP = [
            text.get('temperature', lang), 
            text.get('humidity', lang), 
            text.get('pressure', lang)
        ];

        let displaySensorTypesTH = [
            text.get('temperature', lang), 
            text.get('humidity', lang)
        ];

        let displaySensorTypesTP = [
            text.get('temperature', lang), 
            text.get('pressure', lang)
        ];

        let fields = [];
        for(let i=1; i<=8; i++) fields.push(text.get('field', lang) + ' ' + i);

        let wTypes = [
            text.get('temperature', lang) + ' 0', text.get('temperature', lang) + ' 1', 
            text.get('temperature', lang) + ' 2', text.get('temperature', lang) + ' 3', 
            text.get('temperature', lang) + ' 4', text.get('humidity', lang),
            text.get('pressure', lang)
        ];
        let wSensors = [];
        for(let i=0; i<2; i++) wSensors.push(text.get('wirelessSensor', lang) + ' ' + i);

        let sensor;

        return (<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                {this.props.config.hasOwnProperty('display') &&
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('display', lang) + ' ' + (Number(this.props.num) + 1)} />
            
                    <div className="row justify-content-center">

                        {/* Display type */}
                        <div className="col border border-secondary rounded m-4 pb-3 disp-card">
                            <SelectInput value={`display.type.${this.props.num}`}
                                label={text.get('displayType', lang)}
                                options={displayTypes} 
                                config={this.props.config} 
                                changedConfig={this.changedConfig}
                            />
                            {/* Use "sacrificial" LED switch */}
                            <div className="mt-3">
                                <SwitchInput label={text.get('sacrificial', lang)}
                                    value={'display.sled'}
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig}
                                />
                            </div><hr />
                            {/* Brightness min limit */}
                            <RangeInput value={`display.brightness.min.${this.props.num}`}
                                label={text.get('minimumBrightnessLimit', lang)}
                                min="0"
                                max="255"
                                limitMin="0"
                                limitMax={this.props.config.display.brightness.max[1]}
                                step="1"
                                config={this.props.config} 
                                changedConfig={this.changedConfig}
                                callback={this.setBrightLimit}
                            />
                            {/* Brightness max limit */}
                            <RangeInput value={`display.brightness.max.${this.props.num}`}
                                label={text.get('maximumBrightnessLimit', lang)}
                                min="0"
                                max="255"
                                limitMin={this.props.config.display.brightness.min[1]}
                                limitMax="255"
                                step="1"
                                config={this.props.config} 
                                changedConfig={this.changedConfig}
                                callback={this.setBrightLimit}
                            />
                            <div className="mt-4">
                                {text.get('maximumDisplayCurrent', lang)}: <span className="indication">{
                                    (this.props.config.display.type[this.props.num] > 0) ?
                                    Math.round(((this.props.config.display.type[this.props.num]) * 28 + 2) * 60 * 
                                    (this.props.config.display.brightness.max[this.props.num] / 255)) +
                                    (this.props.config.display.sled ? 1 : 0) : '0'
                                } {text.get('ma', lang)}</span>
                            </div>
                        </div>
                        
                        {/* Brightness */}
                        <DisplayBrightness config={this.props.config} data={this.props.data} num="1" changedConfig={this.changedConfig} />
                    
                        {/* Display auto-off */}
                        <DisplayAutoOff config={this.props.config} num="1" changedConfig={this.changedConfig} />

                        {/* Animation */}
                        <div className="col border border-secondary rounded m-4 pb-3 disp-card">
                            
                            {/* Type */}
                            <SelectInput value={'display.animation.type'}
                                label={text.get('animation', lang)}
                                options={animations} 
                                config={this.props.config} 
                                changedConfig={this.changedConfig}
                                callback={this.changeAnimType}
                            />

                            {/* Animation speed */}
                            <RangeInput value={'display.animation.speed'}
                                label={text.get('animationSpeed', lang)}
                                min="1"
                                max="30"
                                limitMin="1"
                                limitMax="30"
                                step="1"
                                config={this.props.config} 
                                changedConfig={this.changedConfig}
                                callback={this.changeAnimSpeed}
                            />

                            {/* Clock points */}
                            <SelectInput value={'display.animation.points'}
                                label={text.get('clockPoints', lang)}
                                options={clockPoints} 
                                config={this.props.config} 
                                changedConfig={this.changedConfig}
                                callback={this.changeClockPoints}
                            />
                        </div>
                    </div>
                
                    {/* Display settings table */}
                    <div className="row justify-content-center">
                        {[...Array(8)].map((x, i) =>
                            <div key={i} className="col border border-secondary rounded m-4 p-3 disp-card">
                                <h5>{`${text.get('timeSlot', lang)} ${i + 1}`}</h5>

                                {/* Display duration */}
                                <NumberInput value={`display.timeSlot.period.${i}`}
                                    label={text.get('displayDuration', lang)} 
                                    min="0"
                                    max="99"
                                    step="1"
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig}
                                />

                                {/* Sensor color */}
                                <ColorInput value={`display.timeSlot.color.${i}`}
                                    label={text.get('displayColor', lang)} 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig}
                                    num={i}
                                />

                                {/* Sensor to display */}
                                <SelectInput value={`display.timeSlot.sensor.${i}`}
                                    label={text.get('dataSource', lang)}
                                    options={displaySensors} 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig}
                                />

                                {/* Data type to display */}
                                <div className="d-none">
                                    {sensor = this.props.config.display.timeSlot.sensor[i]}
                                </div>
                                {((sensor >= 2 && sensor <= 5) || sensor == 8 || sensor == 9) && <>

                                    {/* Data types: temperature, humidity, pressure */}
                                    {(sensor == 2 || sensor == 8 || sensor == 9) &&
                                        <SelectInput value={`display.timeSlot.data.${i}`}
                                            label={text.get('sensorType', lang)}
                                            options={displaySensorTypesTHP} 
                                            config={this.props.config} 
                                            changedConfig={this.changedConfig}
                                        />
                                    }

                                    {/* Data types: temperature, humidity */}
                                    {(sensor == 4 || sensor == 5) &&
                                        <SelectInput value={`display.timeSlot.data.${i}`}
                                            label={text.get('sensorType', lang)}
                                            options={displaySensorTypesTH} 
                                            config={this.props.config} 
                                            changedConfig={this.changedConfig}
                                        />
                                    }

                                    {/* Data types: temperature, pressure */}
                                    {sensor == 3 &&
                                        <SelectInput value={`display.timeSlot.data.${i}`}
                                            label={text.get('sensorType', lang)}
                                            options={displaySensorTypesTP} 
                                            config={this.props.config} 
                                            changedConfig={this.changedConfig}
                                        />
                                    }

                                    {/* Thingspeak */}
                                    {sensor == 8 &&
                                        <SelectInput value={`display.timeSlot.thing.${i}`}
                                            label={text.get('field', lang)}
                                            options={fields} 
                                            config={this.props.config} 
                                            changedConfig={this.changedConfig}
                                        />
                                    }
                                </>}

                                {/* Wireless sensor */}
                                {sensor == 10 && <>
                                    <SelectInput value={`display.timeSlot.wsensor.num.${i}`}
                                        label={text.get('wirelessSensorNumber', lang)}
                                        options={wSensors} 
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig}
                                    />
                                    <SelectInput value={`display.timeSlot.wsensor.type.${i}`}
                                        label={text.get('sensorType', lang)}
                                        options={wTypes} 
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig}
                                    />
                                </>}
                            </div>
                        )}
                    </div>
                </div>}
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>);
    };
}

export default Display;