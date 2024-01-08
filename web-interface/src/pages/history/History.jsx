import HistoryFn from "./HistoryFn";
import HistoryBlock from "../../fragments/HistoryBlock";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import TextInput from "../../fragments/TextInput";
import SelectInput from "../../fragments/SelectInput";
import NumberInput from "../../fragments/NumberInput";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class History extends HistoryFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        let fields = [
            text.get('temperatureOut', lang), 
            text.get('humidityOut', lang), 
            text.get('pressure', lang), 
            text.get('temperatureIn', lang), 
            text.get('humidityIn', lang),
            text.get('indexForAirQuality', lang),
            'CO2'
        ];

        let sensors = [
            ['--', text.get('forecast', lang), text.get('wirelessSensor', lang), 'thingspeak', 'BME280', 'BMP180', 'SHT21', 'DHT22', 'DS18B20', 'BME680'],
            ['--', text.get('forecast', lang), text.get('wirelessSensor', lang), 'thingspeak', 'BME280', 'SHT21', 'DHT22', 'BME680'],
            ['--', text.get('forecast', lang), text.get('wirelessSensor', lang), 'thingspeak', 'BME280', 'BMP180', 'BME680'],
            ['--', text.get('forecast', lang), text.get('wirelessSensor', lang), 'thingspeak', 'BME280', 'BMP180', 'SHT21', 'DHT22', 'DS18B20', 'BME680'],
            ['--', text.get('forecast', lang), text.get('wirelessSensor', lang), 'thingspeak', 'BME280', 'SHT21', 'DHT22', 'BME680'],
            ['--', 'BME680'],
            ['--', text.get('wirelessSensor', lang)]
        ];

        let wTemperatures = [];
        for(let i=0; i<5; i++) wTemperatures.push(text.get('temperature', lang) + ' ' + i);

        let wSensors = [];
        for(let i=0; i<2; i++) wSensors.push(text.get('wirelessSensor', lang) + ' ' + i);

        let tFields = [];
        for(let i=1; i<8; i++) tFields.push(text.get('field', lang) + ' ' + i);

        let colors = ['FFCC00', '00FFFF', 'FF00FF', 'FFCC00', '00FFFF', 'FF7700', '0000FF'];

        return(<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5" style={{maxWidth: '1280px'}}>
                    <PageHeader h="2" text={text.get('weatherHistory', lang)} />

                    {this.props.config.hasOwnProperty('history') && 
                    <div className="row">
                        <div className="col p-4" style={{minWidth: '250px'}}>
                            <div className="border border-secondary rounded p-3 h-100 sensor">
                                
                                {/* History repository */}
                                <SelectInput value="0"
                                    label={text.get('historyRepository', lang)} 
                                    options={["thingspeak.com"]} 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />

                                {/* Period */}
                                <NumberInput label={text.get('periodMinutes', lang)}
                                    value={'history.period'}
                                    min="1"
                                    max="999"
                                    step="1"
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig}    
                                />

                                {/* Channel ID */}
                                <TextInput label="Channel ID"
                                    maxLength="20" 
                                    placeholder="Channel ID" 
                                    value="history.channelID" 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />
                                
                                {/* Write API Key */}
                                <TextInput label="Write API Key"
                                    maxLength="32" 
                                    placeholder="Write API Key" 
                                    value="history.wrkey" 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />
                                
                                {/* Read API Key */}
                                <TextInput label="Read API Key"
                                    maxLength="32" 
                                    placeholder="Read API Key" 
                                    value="history.rdkey" 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />
                            </div>
                        </div>
                        
                        {[...Array(7)].map((x, i) => 
                            <HistoryBlock key={i}
                                num={i}
                                fields={fields[i]}
                                sensors={sensors[i]}
                                wSensors={wSensors}
                                wTemperatures={wTemperatures}
                                tFields={tFields}
                                bgcolor={window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches ? '000' : 'FFF'}
                                color={colors[i]}
                                config={this.props.config} 
                                changedConfig={this.changedConfig}
                            />
                        )}
                    </div>}
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>)
    };
};

export default History;