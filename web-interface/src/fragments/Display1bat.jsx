import Save from "../pages/Save";
import Languages from "../Languages";
import SelectInput from "./SelectInput";
import Display1wSens from "./Display1wSensor";

class Display1bat extends Save {
    constructor(props) {
        super(props);
        this.changedConfig = this.changedConfig.bind(this);
    }
    
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let fields = [];
        for(let i=1; i<=8; i++) fields.push(text.get('field', lang) + ' ' + i);
        let sensorsBat = ['--', text.get('wirelessSensor', lang), "Thingspeak"];

        return (
            <div className="col border border-secondary rounded m-4 p-3 disp-card">
                <h5>{text.get('batterySymbol', lang)}</h5>
                <SelectInput value={`display.source.bat.sens`}
                    label={text.get('dataSource', lang)}
                    options={sensorsBat}
                    config={this.props.config} 
                    changedConfig={this.props.changedConfig}
                />

                {/* Wireless sensor */}
                {this.props.config.display.source.bat.sens == 1 &&
                    <Display1wSens wSensNum={'display.source.bat.wsensNum'} 
                        config={this.props.config} 
                        changedConfig={this.props.changedConfig} 
                    />
                }

                {/* Thingspeak */}
                {this.props.config.display.source.bat.sens == 2 && <><hr />
                    <SelectInput value={`display.source.bat.thing`}
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

export default Display1bat;