import React from "react";
import SelectInput from "./SelectInput";
import Languages from "../Languages";

class HistoryBlock extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            width: 250
        }

        this.setChartWidth = this.setChartWidth.bind(this);
    }

    setChartWidth(width) {
        if(width > 550) width = 450;
        else width -= 100;
        this.setState({width: width});
    }

    componentDidMount() {
        this.setChartWidth(document.querySelector('.thchart').offsetWidth);

        window.addEventListener('resize', () => {
            this.setChartWidth(document.querySelector('.thchart').offsetWidth);
        });
    }

    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        return(
            <div className="col col-12 col-lg-6 p-4 thchart">
                <div className="border border-secondary rounded p-3 h-100 sensor">
                    <div className="w-100 text-center">
                        <iframe width={this.state.width} 
                            height="261" 
                            style={{border: '1px solid #cccccc'}} 
                            src={`https://thingspeak.com/channels/${this.props.config.history.channelID}/charts/${Number(this.props.num)+1}?bgcolor=%23${this.props.bgcolor}&color=%23${this.props.color}&dynamic=true&results=24&round=2&title=${this.props.fields}&type=line&api_key=${this.props.config.history.rdkey}&width=${this.state.width}`}>
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

                    {this.props.config.history.fields[this.props.num] == 1 && this.props.num == 6 && <>
                        <SelectInput value={`history.wSensors.${this.props.num}`}
                            label={text.get('wirelessSensorNumber', lang)}
                            options={this.props.wSensors} 
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