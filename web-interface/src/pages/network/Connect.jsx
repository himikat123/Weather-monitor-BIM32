import Languages from "../../Languages";
import Network from "../../fragments/Network";
import IpField from "../../fragments/IpField";
import PageHeader from "../../fragments/PageHeader";
import SelectInput from "../../fragments/SelectInput";
import ConnectFn from "./ConnectFn";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Connect extends ConnectFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let card = "col border border-secondary rounded m-4 p-3 tile";

        return (<>
            <Menu language={lang} />

            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('connections', lang)} />
                    
                    {/* Network cards */}
                    <div className="row">
                        <Network num="0" 
                            data={this.props.data} 
                            config={this.props.config} 
                            changedConfig={this.changedConfig} 
                        />
                        <Network num="1" 
                            data={this.props.data} 
                            config={this.props.config} 
                            changedConfig={this.changedConfig} 
                        />
                        <Network num="2" 
                            data={this.props.data} 
                            config={this.props.config} 
                            changedConfig={this.changedConfig} 
                        />
                    </div>

                    <PageHeader h="4" text={text.get('connectionOptions', lang)} />

                    {/* Connection options */}
                    <div className="row">

                        {/* Connection type */}
                        <div className={card}>
                            <SelectInput
                                label={text.get('connectionType', lang)} 
                                value="network.type"
                                options={[text.get('dynamicIp', lang), text.get('staticIp', lang)]} 
                                config={this.props.config} 
                                changedConfig={this.changedConfig} 
                            />
                        </div>

                        <div className={card}>

                            {/* IP address */}
                            <IpField label={text.get('ipAddress', lang)} 
                                data={this.props.data} 
                                config={this.props.config} 
                                value="network.ip"
                                valueDyn="ip" 
                                changedConfig={this.changedConfig} 
                            />

                            {/* Subnet mask */}
                            <IpField label={text.get('subnetMask', lang)} 
                                data={this.props.data} 
                                config={this.props.config} 
                                value="network.mask"
                                valueDyn="mask" 
                                changedConfig={this.changedConfig} 
                            />

                            {/* Default gateway */}
                            <IpField label={text.get('defaultGateway', lang)} 
                                data={this.props.data} 
                                config={this.props.config} 
                                value="network.gw"
                                valueDyn="gw" 
                                changedConfig={this.changedConfig} 
                            />
                        </div>
                        <div className={card}>

                            {/* Preferred DNS */}
                            <IpField label={text.get('preferredDns', lang)} 
                                data={this.props.data} 
                                config={this.props.config} 
                                value="network.dns1"
                                valueDyn="dns1" 
                                changedConfig={this.changedConfig} 
                            />

                            {/* Alternative DNS */}
                            <IpField label={text.get('alternativeDns', lang)} 
                                data={this.props.data} 
                                config={this.props.config} 
                                value="network.dns2"
                                valueDyn="dns2" 
                                changedConfig={this.changedConfig} 
                            />
                        </div>
                    </div>
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>);
    };
};

export default Connect;