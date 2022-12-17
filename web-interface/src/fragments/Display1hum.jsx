import Save from "../pages/Save";
import Languages from "../Languages";
import SelectInput from "./SelectInput";
import Display1wSens from "./Display1wSensor";
import Display1sequence from "./Display1sequence";

class Display1hum extends Save {
    constructor(props) {
        super(props);
        this.state = {
            sourceWas: 0
        };
        this.changedConfig = this.changedConfig.bind(this);
        this.sourceChanged = this.sourceChanged.bind(this);
    }

    sourceChanged(event) {
        if(event.display.source.humIn.sens == 6) 
            this.props.config.display.source.tempIn.sens = 8;
        if(this.state.sourceWas == 6 && event.display.source.humIn.sens != 6)
            this.props.config.display.source.tempIn.sens = 0;
        this.setState({sourceWas: event.display.source.humIn.sens});
        this.props.changedConfig(event);
    }

    componentDidUpdate() {
        if(this.state.sourceWas != this.props.config.display.source.humIn.sens)
            this.setState({sourceWas: this.props.config.display.source.humIn.sens});
    }
    
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let fields = [];
        for(let i=1; i<=8; i++) fields.push(text.get('field', lang) + ' ' + i);
        let sensorsHumIn = [text.get('forecast', lang), text.get('wirelessSensor', lang),
          "Thingspeak", "BME280", "SHT21", "DHT22", text.get('sequence', lang)];
        let sensorsHumOut = [text.get('forecast', lang), text.get('wirelessSensor', lang),
          "Thingspeak", "BME280", "SHT21", "DHT22"];
        let sensType = this.props.place == 'In' ? 
            this.props.config.display.source.humIn.sens : 
            this.props.config.display.source.humOut.sens;

        return (
            <div className="col border border-secondary rounded m-4 p-3 disp-card d-flex align-content-between flex-wrap">
                <div className="w-100">
                    <h5>{text.get(`humidity${this.props.place}`, lang)}</h5>
                    <SelectInput value={`display.source.hum${this.props.place}.sens`}
                        label={text.get('dataSource', lang)}
                        options={this.props.place == 'In' ? sensorsHumIn : sensorsHumOut}
                        config={this.props.config} 
                        changedConfig={this.sourceChanged}
                    />

                    {/* Wireless sensor */}
                    {sensType == 1 &&
                        <Display1wSens opts=""
                            wSensNum={`display.source.hum${this.props.place}.wsensNum`} 
                            wSens={`display.source.hum${this.props.place}.wsens`}
                            config={this.props.config} 
                            changedConfig={this.props.changedConfig} 
                        />
                    }

                    {/* Thingspeak */}
                    {sensType == 2 && <><hr />
                        <SelectInput value={`display.source.hum${this.props.place}.thing`}
                            label={text.get('field', lang)}
                            options = {fields}
                            config={this.props.config} 
                            changedConfig={this.props.changedConfig}
                        />
                    </>}

                    {/* Sequence */}
                    {sensType == 6 &&
                        <Display1sequence type="hum"
                            config={this.props.config} 
                            changedConfig={this.props.changedConfig} 
                        />
                    }
                </div>
                {sensType == 6 &&
                    <div className="mt-3">* {text.get('leaveTheNameBlankToNotDisplayThisTimeslot', lang)}</div>
                }
            </div>
        )
    }
};

export default Display1hum;