import Save from "../pages/Save";
import Languages from "../Languages";
import SelectInput from "./SelectInput";

class Display1wSens extends Save {
    constructor(props) {
        super(props);
        this.changedConfig = this.changedConfig.bind(this);
    }
    
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let wSensors = [];
        for(let i=0; i<2; i++) wSensors.push(text.get('wirelessSensor', lang) + ' ' + i);

        return (<><hr />
            <SelectInput value={this.props.wSensNum}
                label={text.get('wirelessSensorNumber', lang)}
                options={wSensors} 
                config={this.props.config} 
                changedConfig={this.props.changedConfig}
            />
            {this.props.optsTemp && 
            <SelectInput value={this.props.temp}
                label={text.get('temperatureSensorNumber', lang)}
                options={this.props.optsTemp} 
                config={this.props.config} 
                changedConfig={this.props.changedConfig}
            />}
            {this.props.optsVolt && 
            <SelectInput value={this.props.volt}
                label={text.get('sensorType', lang)}
                options={this.props.optsVolt} 
                config={this.props.config} 
                changedConfig={this.props.changedConfig}
            />}
        </>)
    }
};

export default Display1wSens;