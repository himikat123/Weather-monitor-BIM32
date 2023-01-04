import Save from "../pages/Save";
import Languages from "../Languages";
import SelectInput from "./SelectInput";
import Display1wSens from "./Display1wSensor";

class Display1volt extends Save {
    constructor(props) {
        super(props);
        this.changedConfig = this.changedConfig.bind(this);
    }
    
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let fields = [];
        for(let i=1; i<=8; i++) fields.push(text.get('field', lang) + ' ' + i);
        let sensorsVolt = ['--', text.get('wirelessSensor', lang), "Thingspeak"];

        return (
            <div className="col border border-secondary rounded m-4 p-3 disp-card">
                <h5>{text.get('voltage', lang)}</h5>
                <SelectInput value={`display.source.volt.sens`}
                    label={text.get('dataSource', lang)}
                    options={sensorsVolt}
                    config={this.props.config} 
                    changedConfig={this.props.changedConfig}
                />

                {/* Wireless sensor */}
                {this.props.config.display.source.volt.sens == 1 &&
                    <Display1wSens optsVolt={[
                            text.get('batteryVoltage', lang), 
                            text.get('batteryPercentage', lang),  
                            text.get('voltage', lang) + ' PZEM-004t'
                        ]}
                        wSensNum={'display.source.volt.wsensNum'} 
                        volt={'display.source.volt.volt'}
                        config={this.props.config} 
                        changedConfig={this.props.changedConfig} 
                    />
                }

                {/* Thingspeak */}
                {this.props.config.display.source.volt.sens == 2 && <><hr />
                    <SelectInput value={`display.source.volt.thing`}
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

export default Display1volt;