import SensorsFn from "./SensorsFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import Sensor from "../../fragments/Sensor";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Sensors extends SensorsFn{

    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        return (<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('sensors', lang)} />

                    <div className="row">
                        <Sensor name="bme280" t="1" h="1" p="1" l="0" a="0" 
                            data={this.state.data} 
                            config={this.props.config} 
                            changedConfig={this.changedConfig} 
                        />
                        <Sensor name="bmp180" t="1" h="0" p="1" l="0" a="0" 
                            data={this.state.data} 
                            config={this.props.config} 
                            changedConfig={this.changedConfig} 
                        />
                        <Sensor name="sht21" t="1" h="1" p="0" l="0" a="0" 
                            data={this.state.data} 
                            config={this.props.config} 
                            changedConfig={this.changedConfig} 
                        />
                        <Sensor name="dht22" t="1" h="1" p="0" l="0" a="0" 
                            data={this.state.data} 
                            config={this.props.config} 
                            changedConfig={this.changedConfig} 
                        />
                        <Sensor name="ds18b20" t="1" h="0" p="0" l="0" a="0" 
                            data={this.state.data} 
                            config={this.props.config} 
                            changedConfig={this.changedConfig} 
                        />
                        <Sensor name="esp32" t="1" h="0" p="0" l="0" a="0" 
                            data={this.state.data} 
                            config={this.props.config} 
                            changedConfig={this.changedConfig} 
                        />
                        <Sensor name="max44009" t="0" h="0" p="0" l="1" a="0" 
                            data={this.state.data} 
                            config={this.props.config} 
                            changedConfig={this.changedConfig} 
                        />
                        <Sensor name="bh1750" t="0" h="0" p="0" l="1" a="0" 
                            data={this.state.data} 
                            config={this.props.config} 
                            changedConfig={this.changedConfig} 
                        />
                        <Sensor name="analog" t="0" h="0" p="0" l="0" a="1" 
                            data={this.state.data} 
                            config={this.props.config} 
                            changedConfig={this.changedConfig} 
                        />
                    </div>
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>);
    };
};

export default Sensors;