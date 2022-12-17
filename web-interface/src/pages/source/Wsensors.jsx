import WsensorsFn from "./WsensorsFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import RangeInput from "../../fragments/RangeInput";
import SelectInput from "../../fragments/SelectInput";
import NumberInput from "../../fragments/NumberInput";
import Wsensor from "../../fragments/Wsensor";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Wsensors extends WsensorsFn{
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let batTypes = [text.get('threeBatteries', lang), text.get('liIonBattery', lang)];

        return (<>
            <Menu language={lang} /> 
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    {this.props.config.hasOwnProperty('wsensor') && <>
                        <PageHeader h="2" text={text.get('wirelessSensors', lang)} />

                        <div className="row justify-content-center">
                        {[...Array(2)].map((x, num) => <div key={num} className="col col-12 col-xl-6">
                            <div className="border border-secondary rounded m-4 p-4 sensor">
                                <PageHeader h="5" text={text.get('wirelessSensor', lang) + ': ' + num} />
                                <p className="text-center">{text.get('dataFrom', lang)} <span className="indication">
                                    {this.getTime(num)}</span>
                                </p>
                                <div className="row">
                                    <div className="col col-5 text-center d-none d-sm-block">
                                        {text.get('sensor', lang)}
                                    </div>
                                    <div className="col col-7 text-center d-none d-sm-block">
                                        {text.get('correction', lang)}
                                    </div>
                                </div>

                                {/* Temperatures */}
                                {[...Array(5)].map((x, i) =>
                                    <Wsensor name={text.get('temperature', lang) + ' ' + i} 
                                        value={this.getTemp(num, this.props.config.wsensor.temp.corr[num][i], i)}
                                        corr={`wsensor.temp.corr.${num}.${i}`}
                                        min="-10" max="10" step="0.1" key={i}
                                        config={this.props.config} changedConfig={this.changedConfig}
                                        units="°C"
                                    />
                                )}

                                {/* Humidity */}
                                <Wsensor name={text.get('humidity', lang)} 
                                    value={this.getVal(num, this.props.config.wsensor.hum.corr[num], '%', 'hum', 0, 100, 10)}
                                    corr={`wsensor.hum.corr.${num}`}
                                    min="-10" max="10" step="0.1"
                                    config={this.props.config} changedConfig={this.changedConfig}
                                    units="%"
                                />

                                {/* Pressure */}
                                <Wsensor name={text.get('pressure', lang)} 
                                    value={this.getVal(num, this.props.config.wsensor.pres.corr[num], text.get('mm', lang), 'pres', 450, 1200, 10)}
                                    corr={`wsensor.pres.corr.${num}`}
                                    min="-10" max="10" step="0.1"
                                    config={this.props.config} changedConfig={this.changedConfig}
                                    units={text.get('mm', lang)}
                                />

                                {/* Ambient light */}
                                <Wsensor name={text.get('ambientLight', lang)} 
                                    value={this.getVal(num, this.props.config.wsensor.light.corr[num], text.get('lux', lang), 'light', 0, 200000, 10)}
                                    corr={`wsensor.light.corr.${num}`}
                                    min="-10" max="10" step="0.1"
                                    config={this.props.config} changedConfig={this.changedConfig}
                                    units={text.get('lux', lang)}
                                />

                                {/* Voltage PZEM-004t */}
                                <Wsensor name={text.get('voltage', lang)} 
                                    value={this.getVal(num, this.props.config.wsensor.volt.corr[num], text.get('v', lang), 'voltage', 80, 260, 10)}
                                    corr={`wsensor.volt.corr.${num}`}
                                    min="-10" max="10" step="0.1"
                                    config={this.props.config} changedConfig={this.changedConfig}
                                    units={text.get('v', lang)}
                                />

                                {/* Current PZEM-004t */}
                                <Wsensor name={text.get('current', lang)} 
                                    value={this.getVal(num, this.props.config.wsensor.curr.corr[num], 'A', 'current', 0, 100, 1000)}
                                    corr={`wsensor.curr.corr.${num}`}
                                    min="-1" max="1" step="0.001"
                                    config={this.props.config} changedConfig={this.changedConfig}
                                    units={text.get('a', lang)}
                                />
                                
                                {/* Power PZEM-004t */}
                                <Wsensor name={text.get('power', lang)} 
                                    value={this.getVal(num, this.props.config.wsensor.pow.corr[num], text.get('w', lang), 'power', 0, 23000, 10)}
                                    corr={`wsensor.pow.corr.${num}`}
                                    min="-10" max="10" step="0.1"
                                    config={this.props.config} changedConfig={this.changedConfig}
                                    units={text.get('w', lang)}
                                />

                                {/* Energy PZEM-004t */}
                                <Wsensor name={text.get('energy', lang)} 
                                    value={this.getVal(num, this.props.config.wsensor.enrg.corr[num], text.get('wh', lang), 'energy', 0, 10000000, 1)}
                                    corr={`wsensor.enrg.corr.${num}`}
                                    min="-10" max="10" step="1"
                                    config={this.props.config} changedConfig={this.changedConfig}
                                    units={text.get('wh', lang)}
                                />
                                
                                {/* Frequence PZEM-004t */}
                                <Wsensor name={text.get('frequency', lang)} 
                                    value={this.getVal(num, this.props.config.wsensor.freq.corr[num], text.get('hz', lang), 'freq', 45, 65, 10)}
                                    corr={`wsensor.freq.corr.${num}`}
                                    min="-10" max="10" step="0.1"
                                    config={this.props.config} changedConfig={this.changedConfig}
                                    units={text.get('hz', lang)}
                                /><hr />

                                {/* Battery */}
                                <div className="row align-items-center">
                                    <div className="col col-12 col-sm-5">
                                        <p className="text-center my-2">
                                            {text.get('batteryVoltage', lang)}<br /><span className="indication">
                                                {this.getBat(num, text.get('v', lang))}
                                            </span>
                                        </p>
                                    </div>
                                    <div className="col col-12 col-sm-7">
                                        <RangeInput value={`wsensor.bat.k.${num}`}
                                            min="10" 
                                            max="250"
                                            limitMin="10"
                                            limitMax="250" 
                                            step="0.2"
                                            config={this.props.config} 
                                            changedConfig={this.props.changedConfig}
                                        />
                                    </div>
                                </div>

                                {/* Battery type */}
                                <SelectInput value={`wsensor.bat.type.${num}`}
                                    label={text.get('sourceOfPower', lang)} options={batTypes} 
                                    config={this.props.config} changedConfig={this.changedConfig}
                                />

                                {/* Channel number */}
                                <NumberInput min="1" max="100" step="1" 
                                    placeholder={text.get('channelNumber', lang)}
                                    label={text.get('channelNumber', lang)}
                                    value={'wsensor.channel'}
                                    config={this.props.config} changedConfig={this.changedConfig} 
                                />

                                {/* Data expiration time */}
                                <NumberInput min="1" max="100" step="1" 
                                    placeholder={text.get('dataExpirationTime', lang)}
                                    label={text.get('dataExpirationTime', lang)}
                                    value={`wsensor.expire.${num}`} 
                                    config={this.props.config} changedConfig={this.changedConfig} 
                                />
                            </div></div>)}
                        </div>
                    </>}
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>);
    };
};

export default Wsensors;