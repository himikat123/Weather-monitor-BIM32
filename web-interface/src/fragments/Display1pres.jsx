import Save from "../pages/Save";
import Languages from "../Languages";
import SelectInput from "./SelectInput";
import Display1wSens from "./Display1wSensor";

class Display1pres extends Save {
    constructor(props) {
        super(props);
        this.changedConfig = this.changedConfig.bind(this);
    }
    
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let fields = [];
        for(let i=1; i<=8; i++) fields.push(text.get('field', lang) + ' ' + i);
        let sensorsPres = [text.get('forecast', lang), text.get('wirelessSensor', lang),
          "Thingspeak", "BME280", "BMP180"];

        return (
            <div className="col border border-secondary rounded m-4 p-3 disp-card">
                <h5>{text.get(`pressure`, lang)}</h5>
                <SelectInput value={`display.source.presOut.sens`}
                    label={text.get('dataSource', lang)}
                    options={sensorsPres}
                    config={this.props.config} 
                    changedConfig={this.props.changedConfig}
                />

                {/* Wireless sensor */}
                {this.props.config.display.source.presOut.sens == 1 &&
                    <Display1wSens opts=""
                        wSensNum={`display.source.presOut.wsensNum`} 
                        wSens={`display.source.presOut.wsens`}
                        config={this.props.config} 
                        changedConfig={this.props.changedConfig} 
                    />
                }

                {/* Thingspeak */}
                {this.props.config.display.source.presOut.sens == 2 && <><hr />
                    <SelectInput value={`display.source.presOut.thing`}
                        label={text.get('field', lang)}
                        options = {fields}
                        config={this.props.config} 
                        changedConfig={this.props.changedConfig}
                    />
                </>}
            </div>
        )
    }
};

export default Display1pres;