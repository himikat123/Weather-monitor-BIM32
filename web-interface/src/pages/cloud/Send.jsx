import SendFn from "./SendFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import SwitchInput from "../../fragments/SwitchInput";
import NumberInput from "../../fragments/NumberInput";
import TextInput from "../../fragments/TextInput";
import SelectInput from "../../fragments/SelectInput";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Send extends SendFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let sensors = [
            '--', text.get('forecast', lang), 'BME280', 'BMP180', 'SHT21', 'DHT22', 'DS18B20', 
            'MAX44009', 'BH1750', text.get('analogInput', lang), 'ESP32', text.get('wirelessSensor', lang)
        ];
        let thp = [text.get('temperature', lang), text.get('humidity', lang), text.get('pressure', lang)];
        let th = [text.get('temperature', lang), text.get('humidity', lang)];
        let tp = [text.get('temperature', lang), text.get('pressure', lang)];
        let tr = [text.get('temperature', lang), text.get('runtime', lang)];
        let wTypes = [
            text.get('temperature', lang) + ' 0', text.get('temperature', lang) + ' 1', text.get('temperature', lang) + ' 2', 
            text.get('temperature', lang) + ' 3', text.get('temperature', lang) + ' 4', text.get('humidity', lang),
            text.get('pressure', lang), text.get('ambientLight', lang), text.get('voltage', lang), text.get('current', lang),
            text.get('power', lang), text.get('energy', lang), text.get('frequency', lang), 
            text.get('batteryVoltage', lang), text.get('batteryLevel', lang)
        ];
        let wSensors = [];
        for(let i=0; i<2; i++) wSensors.push(text.get('wirelessSensor', lang) + ' ' + i);
        let tf = this.props.config.hasOwnProperty('thingspeakSend') ? this.props.config.thingspeakSend.fields : 0;
        let nf = this.props.config.hasOwnProperty('narodmonSend') ? this.props.config.narodmonSend.sensors : 0;

        return (<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('dataSend', lang)} />

                    {this.props.config.hasOwnProperty('thingspeakSend') &&
                    <div className="row justify-content-center">
                        
                        {/* Send to thingspeak.com */}
                        <div className="col border border-secondary rounded m-4 p-3">

                            {/* on/off switch */}
                            <SwitchInput value="thingspeakSend.turnOn"
                                label={text.get('sendDataTo', lang) + ' thingspeak.com'}
                                config={this.props.config} 
                                changedConfig={this.changedConfig}
                            />
                            <div className="row">

                                {/* Period */}
                                <div className="col-12 col-sm-6 col-md-4 col-lg-3 col-xl-2">
                                    <NumberInput label={text.get('periodMinutes', lang)}
                                        value={'thingspeakSend.period'}
                                        min="1"
                                        max="999"
                                        step="1"
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig}    
                                    />
                                </div>

                                {/* Channel ID */}
                                <div className="col-12 col-sm-6 col-md-4 col-lg-3 col-xl-2">
                                    <TextInput label="Channel ID"
                                        maxLength="20" 
                                        placeholder="Channel ID" 
                                        value="thingspeakSend.channelID" 
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig} 
                                    />
                                </div>
                                
                                {/* Write API Key */}
                                <div className="col-12 col-sm-6 col-md-4 col-lg-3 col-xl-2">
                                    <TextInput label="Write API Key"
                                        maxLength="32" 
                                        placeholder="Write API Key" 
                                        value="thingspeakSend.wrkey" 
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig} 
                                    />
                                </div>
                                
                                {/* Read API Key */}
                                <div className="col-12 col-sm-6 col-md-4 col-lg-3 col-xl-2">
                                    <TextInput label="Read API Key"
                                        maxLength="32" 
                                        placeholder="Read API Key" 
                                        value="thingspeakSend.rdkey" 
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig} 
                                    />
                                </div>
                            </div><hr />
                            <div className="row">    
                                {/* Field 1...8 */}
                                {[...Array(8)].map((x, i) =>
                                    <div className="pb-4 col-12 col-sm-6 col-md-4 col-lg-3 col-xl-2" key={i}>
                                        <SelectInput value={`thingspeakSend.fields.${i}`}
                                            label={text.get('field', lang) + ' ' + (i + 1)}
                                            options={sensors} 
                                            config={this.props.config} 
                                            changedConfig={this.changedConfig}
                                        />
                                        {((tf[i] > 0 && tf[i] < 6) || tf[i] == 10) &&
                                            <SelectInput value={`thingspeakSend.types.${i}`}
                                                label={text.get('sensorType', lang)}
                                                options={(tf[i] == 1 || tf[i] == 2) ? thp : tf[i] == 3 ? tp : tf[i] == 4 || tf[i] == 5 ? th : tr} 
                                                config={this.props.config} 
                                                changedConfig={this.changedConfig}
                                            />
                                        }
                                        {tf[i] == 11 && <>
                                            <SelectInput value={`thingspeakSend.wsensors.${i}`}
                                                label={text.get('wirelessSensorNumber', lang)}
                                                options={wSensors} 
                                                config={this.props.config} 
                                                changedConfig={this.changedConfig}
                                            />
                                            <SelectInput value={`thingspeakSend.wtypes.${i}`}
                                                label={text.get('sensorType', lang)}
                                                options={wTypes} 
                                                config={this.props.config} 
                                                changedConfig={this.changedConfig}
                                            />
                                        </>}
                                    </div> 
                                )}
                            </div>
                        </div>
                    </div>}

                    {/* Send to narodmon.ru */}
                    {this.props.config.hasOwnProperty('narodmonSend') &&
                    <div className="row justify-content-center">    
                        <div className="col border border-secondary rounded m-4 p-3">

                            {/* on/off switch */}
                            <SwitchInput value="narodmonSend.turnOn"
                                label={text.get('sendDataTo', lang) + ' narodmon.ru'}
                                config={this.props.config} 
                                changedConfig={this.changedConfig}
                            />
                            <div className="row">

                                {/* Period */}
                                <div className="col-12 col-sm-6 col-md-4 col-lg-3 col-xl-2">
                                    <NumberInput label={text.get('periodMinutes', lang)}
                                        value={'narodmonSend.period'}
                                        min="1"
                                        max="999"
                                        step="1"
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig}    
                                    />
                                </div>

                                {/* MAC address */}
                                <div className="col-12 col-sm-6 col-md-4 col-lg-3 col-xl-2 mt-3">
                                    <label className="form-label">MAC</label>
                                    <div className="form-control mac">
                                        {"BIM" + this.state.mac}
                                    </div>
                                </div>
                                
                                {/* Latitude */}
                                <div className="col-12 col-sm-6 col-md-4 col-lg-3 col-xl-2">
                                    <TextInput label={text.get('latitude', lang)}
                                        maxLength="10" 
                                        placeholder={text.get('latitude', lang)} 
                                        value="narodmonSend.lat" 
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig} 
                                    />
                                </div>
                                
                                {/* Longitude */}
                                <div className="col-12 col-sm-6 col-md-4 col-lg-3 col-xl-2">
                                    <TextInput label={text.get('longitude', lang)}
                                        maxLength="10" 
                                        placeholder={text.get('longitude', lang)} 
                                        value="narodmonSend.lon" 
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig} 
                                    />
                                </div>

                                {/* Sensor name */}
                                <div className="col-12 col-sm-6 col-md-4 col-lg-3 col-xl-2">
                                    <TextInput label={text.get('sensorName', lang)}
                                        maxLength="16" 
                                        placeholder={text.get('sensorName', lang)} 
                                        value="narodmonSend.name" 
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig} 
                                    />
                                </div>

                            </div><hr />
                            <div className="row">
                                
                                {/* Sensor 1...12 */}
                                {[...Array(12)].map((x, i) =>
                                    <div className="pb-4 col-12 col-sm-6 col-md-4 col-lg-3 col-xl-2" key={i}>
                                        <SelectInput value={`narodmonSend.sensors.${i}`}
                                            label={text.get('sensor', lang) + ' ' + (i + 1)}
                                            options={sensors} 
                                            config={this.props.config} 
                                            changedConfig={this.changedConfig}
                                        />
                                        {((nf[i] > 0 && nf[i] < 6) || nf[i] == 10) &&
                                            <SelectInput value={`narodmonSend.types.${i}`}
                                                label={text.get('sensorType', lang)}
                                                options={(nf[i] == 1 || nf[i] == 2) ? thp : nf[i] == 3 ? tp : nf[i] == 4 || nf[i] == 5 ? th : tr} 
                                                config={this.props.config} 
                                                changedConfig={this.changedConfig}
                                            />
                                        }
                                        {nf[i] == 11 && <>
                                            <SelectInput value={`narodmonSend.wsensors.${i}`}
                                                label={text.get('wirelessSensorNumber', lang)}
                                                options={wSensors} 
                                                config={this.props.config} 
                                                changedConfig={this.changedConfig}
                                            />
                                            <SelectInput value={`narodmonSend.wtypes.${i}`}
                                                label={text.get('sensorType', lang)}
                                                options={wTypes} 
                                                config={this.props.config} 
                                                changedConfig={this.changedConfig}
                                            />
                                        </>}
                                        <TextInput label={text.get('sensorMetric', lang)}
                                            maxLength="16" 
                                            placeholder={text.get('sensorMetric', lang)} 
                                            value={`narodmonSend.metrics.${i}`}
                                            config={this.props.config} 
                                            changedConfig={this.changedConfig} 
                                        />
                                    </div> 
                                )}
                            </div>
                        </div>
                    </div>}
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>);
    }
};

export default Send;