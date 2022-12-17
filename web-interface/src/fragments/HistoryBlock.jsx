import React from "react";
import SelectInput from "./SelectInput";
import Languages from "../Languages";

class HistoryBlock extends React.Component {
    constructor(props) {
        super(props);
    }

    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        return(
            <div className="col p-4">
                <div className="border border-secondary rounded p-3 h-100 sensor">
                    <div className="w-100 text-center">
                        <iframe width="450" 
                            height="261" 
                            style={{border: '1px solid #cccccc'}} 
                            src={`https://thingspeak.com/channels/${this.props.config.history.channelID}/charts/${Number(this.props.num)+1}?bgcolor=%23${this.props.bgcolor}&color=%23${this.props.color}&dynamic=true&results=24&round=2&title=${this.props.fields}&type=line&api_key=${this.props.config.history.rdkey}`}>
                        </iframe>
                    </div>
                    <SelectInput value={`history.fields.${this.props.num}`}
                        label={this.props.fields}
                        options={this.props.sensors} 
                        config={this.props.config} 
                        changedConfig={this.props.changedConfig}
                    />
                    {this.props.config.history.fields[this.props.num] == 2 && <>
                        <SelectInput value={`history.wSensors.${this.props.num}`}
                            label={text.get('wirelessSensorNumber', lang)}
                            options={this.props.wSensors} 
                            config={this.props.config} 
                            changedConfig={this.props.changedConfig}
                        />
                        {(this.props.num==0 || this.props.num==3) &&
                            <SelectInput value={`history.wTypes.${this.props.num}`}
                                label={text.get('sensorType', lang)}
                                options={this.props.wTemperatures} 
                                config={this.props.config} 
                                changedConfig={this.props.changedConfig}
                            />
                        }
                    </>}
                    {this.props.config.history.fields[this.props.num] == 3 && <>
                        <SelectInput value={`history.tFields.${this.props.num}`}
                            label={text.get('field', lang)}
                            options={this.props.tFields} 
                            config={this.props.config} 
                            changedConfig={this.props.changedConfig}
                        />
                    </>} 
                </div> 
            </div>
        );
    }
};

export default HistoryBlock;