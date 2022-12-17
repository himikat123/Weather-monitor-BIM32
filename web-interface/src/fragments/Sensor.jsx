import Save from "../pages/Save";
import Languages from "../Languages";
import SensorIndication from "./SensorIndication";

class Sensor extends Save {
    constructor(props) {
        super(props);
        this.state = { data: {} };
        this.getSensorTemp = this.getSensorTemp.bind(this);
        this.getSensorHum = this.getSensorHum.bind(this);
        this.getSensorPres = this.getSensorPres.bind(this);
        this.getSensorLight = this.getSensorLight.bind(this);
    };

    /**
     * checks if the sensor temperature exists and
     * @returns temperature or "--"
     */
    getSensorTemp() {
        try { 
            let sensor = this.props.data[this.props.name].temp;
            if(sensor === undefined || isNaN(sensor) || sensor < -50 || sensor > 100) return '--'
            else return Math.round((sensor + this.props.config.sensors[this.props.name].t) * 10) / 10; 
        }
        catch(e) { return '--' }
    }

    /**
     * checks if the sensor humidity exists and
     * @returns humidity or "--"
     */
    getSensorHum() {
        try { 
            let sensor = this.props.data[this.props.name].hum;
            if(sensor === undefined || isNaN(sensor) || sensor < 0 || sensor > 100) return '--';
            else return Math.round((sensor + this.props.config.sensors[this.props.name].h) * 10) / 10;
        }
        catch(e) { return '--' }
    }

    /**
     * checks if the sensor pressure exists and
     * @returns pressure or "--"
     */
    getSensorPres() {
        try { 
            let sensor = this.props.data[this.props.name].pres;
            if(sensor === undefined || isNaN(sensor) || sensor < 500 || sensor > 1500) return '--';
            else return Math.round((sensor + this.props.config.sensors[this.props.name].p) * 10) / 10;
        }
        catch(e) { return '--' }
    }

    /**
     * checks if the sensor ambient light exists and
     * @returns ambient light or "--"
     */
    getSensorLight() {
        try { 
            let sensor = this.props.data[this.props.name].light;
            if(sensor === undefined || isNaN(sensor) || sensor < 0 || sensor > 200000) return '--';
            else return Math.round((sensor + this.props.config.sensors[this.props.name].l) * 10) / 10;
        }
        catch(e) { return '--' }
    }

    /**
     * checks if analog voltage exists and
     * @returns analog voltage or "--"
     */
    getSensorAnalog() {
        try { 
            let sensor = this.props.data.analog.volt;
            if(sensor === undefined || isNaN(sensor) || sensor < 0 || sensor > 5) return '--';
            return Math.round((sensor + this.props.config.sensors[this.props.name].v) * 10) / 10;
        }
        catch(e) { return '--' }
    }

    /**
     * props have been updated
     * @param {*} nextProps 
     */
    componentDidUpdate(nextProps) {
        if(this.state.data !== nextProps.data) this.setState({ data: nextProps.data });
    }

    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        return(this.props.config.hasOwnProperty('sensors') &&
            <div className="col-12 col-md-6 col-lg-4 p-4">
                <div className="border border-secondary rounded m-2 p-3 sensor h-100">

                    <h4 className="d-flex justify-content-center">
                        {this.props.a == 1 ? text.get('analogInput', lang) : this.props.name.toUpperCase()}
                    </h4><hr />

                    <div className="d-flex flex-column align-items-center">
                        {this.props.t == 1 && 
                            <SensorIndication name={text.get('temperature', lang)} 
                                data={this.getSensorTemp()} 
                                config={this.props.config}
                                changedConfig={this.props.changedConfig}
                                corr={`sensors.${this.props.name}.t`}
                                sensor={this.props.name}
                                units="°C"
                            />
                        }

                        {this.props.h == 1 &&
                            <SensorIndication name={text.get('humidity', lang)} 
                                data={this.getSensorHum()}
                                config={this.props.config}
                                changedConfig={this.props.changedConfig}
                                corr={`sensors.${this.props.name}.h`}
                                sensor={this.props.name}
                                units="%"
                            />
                        }

                        {this.props.p == 1 &&
                            <SensorIndication name={text.get('pressure', lang)} 
                                data={this.getSensorPres()}
                                config={this.props.config}
                                changedConfig={this.props.changedConfig}
                                corr={`sensors.${this.props.name}.p`}
                                sensor={this.props.name}
                                units={text.get('mm', lang)}
                            />
                        }

                        {this.props.l == 1 &&
                            <SensorIndication name={text.get('ambientLight', lang)}
                                data={this.getSensorLight()}
                                config={this.props.config}
                                changedConfig={this.props.changedConfig}
                                corr={`sensors.${this.props.name}.l`}
                                sensor={this.props.name}
                                units={text.get('lux', lang)} 
                            />
                        }

                        {this.props.a == 1 &&
                            <SensorIndication name={text.get('voltage', lang)} 
                                data={this.getSensorAnalog()}
                                config={this.props.config}
                                changedConfig={this.props.changedConfig}
                                corr={`sensors.${this.props.name}.v`}
                                sensor={this.props.name}
                                units={text.get('v', lang)} 
                            />
                        }
                    </div>
                </div>          
            </div>
        );
    }
};

export default Sensor;