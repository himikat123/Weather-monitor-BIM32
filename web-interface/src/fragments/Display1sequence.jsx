import Save from "../pages/Save";
import Languages from "../Languages";
import SelectInput from "./SelectInput";
import TextInput from "./TextInput";
import RangeInput from "./RangeInput";

class Display1sequence extends Save {
    constructor(props) {
        super(props);
        this.changedConfig = this.changedConfig.bind(this);
    }
    
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let fields = [];
        for(let i=1; i<=8; i++) fields.push(text.get('field', lang) + ' ' + i);
        let wSensors = [];
        for(let i=0; i<2; i++) wSensors.push(text.get('wirelessSensor', lang) + ' ' + i);
        let wsensorTemp = [];
        for(let i=0; i<5; i++) wsensorTemp.push(text.get('temperature', lang) + ' ' + i);
        let type = 0;
        if(this.props.type == 'temp') type = this.props.config.display.source.sequence.temp;
        if(this.props.type == 'hum') type = this.props.config.display.source.sequence.hum;
        let tempSensors = [
            '--', 
            'Thingspeak', 
            text.get('wirelessSensor', lang),
            'BME280', 
            'BMP180', 
            'SHT21', 
            'DHT22', 
            'DS18B20', 
            'ESP32',
            text.get('forecast', lang)
        ];

        let humSensors = [
            '--', 
            'Thingspeak', 
            text.get('wirelessSensor', lang),
            'BME280',
            'SHT21', 
            'DHT22',
            text.get('forecast', lang)
        ];

        return (<><hr />
            {/* display timeslot duration */}
            <RangeInput value={`display.source.sequence.dur`}
                label={text.get('displayDuration', lang)} 
                min="1"
                max="20"
                limitMin="1"
                limitMax="20"
                step="1"
                config={this.props.config} 
                changedConfig={this.changedConfig}
            />

            {[...Array(4)].map((x, i) => <div key={i}><hr />
                
                {/* Sensor select */}
                <SelectInput value={`display.source.sequence.${this.props.type}.${i}`}
                    label={text.get('timeSlot', lang) + ' ' + (i + 1)}
                    options={this.props.type == 'temp' ? tempSensors : humSensors}
                    config={this.props.config} 
                    changedConfig={(this.props.changedConfig)}
                />

                {/* Thingspeak */}
                {type[i] == 1 &&
                    <SelectInput value={`display.source.sequence.thng${this.props.type}.${i}`}
                        label={text.get('field', lang)}
                        options={fields}
                        config={this.props.config} 
                        changedConfig={this.props.changedConfig}
                    />
                }

                {/* Wireless sensor */}
                {type[i] == 2 && <>
                    <SelectInput value={`display.source.sequence.wsens${this.props.type}.${i}.0`}
                        label={text.get('wirelessSensorNumber', lang)}
                        options={wSensors}
                        config={this.props.config} 
                        changedConfig={this.props.changedConfig}
                    />
                    {this.props.type == 'temp' &&
                    <SelectInput value={`display.source.sequence.wsenstemp.${i}.1`}
                        label={text.get('temperatureSensorNumber', lang)}
                        options={wsensorTemp}
                        config={this.props.config} 
                        changedConfig={this.props.changedConfig}
                    />}
                </>}

                {/* Sensor name */}
                <TextInput maxLength="15"
                    label={text.get('name', lang) + " *"} 
                    placeholder={text.get('name', lang)} 
                    value={`display.source.sequence.name.${i}`}
                    config={this.props.config} 
                    changedConfig={this.props.changedConfig} 
                />
            </div>)}
        </>)
    }
};

export default Display1sequence;