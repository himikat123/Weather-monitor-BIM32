import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import PasswordField from "../../fragments/PasswordField";
import IpField from "../../fragments/IpField";
import AccessPointFn from "./AccessPointFn";
import TextInput from "../../fragments/TextInput";
import NumberInput from "../../fragments/NumberInput";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class AccessPoint extends AccessPointFn{
    render(){
        let text = new Languages();
        let lang = this.props.config.lang;

        return (<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('accessPoint', lang)} />
                    
                    <div className="row d-flex justify-content-center">

                        {/* Access point name & password */}
                        <div className="col border border-secondary rounded m-4 p-3 tile account">
                            <TextInput label={text.get('accessPointName', lang)}
                                maxLength="32"
                                placeholder={text.get('accessPointName', lang)}
                                value="accessPoint.ssid"
                                config={this.props.config} 
                                changedConfig={this.changedConfig}  
                            />
                            <PasswordField label={text.get('password', lang)}
                                num="ap" 
                                config={this.props.config} 
                                changedConfig={this.changedConfig} 
                            />
                        </div>

                        {/* Access point IP address & subnet mask  */}
                        <div className="col border border-secondary rounded m-4 p-3 tile" style={{display: 'none'}}>
                            <IpField label={text.get('ipAddress', lang)} 
                                id="apIp" 
                                data={this.state.data} 
                                config={this.props.config} 
                                value="ip" 
                                changedConfig={this.changedConfig} 
                            />
                            <IpField label={text.get('subnetMask', lang)} 
                                id="apMask" 
                                data={this.state.data} 
                                config={this.props.config} 
                                value="mask" 
                                changedConfig={this.changedConfig} 
                            />
                        </div>

                        {/* Access point wireless channel number */}
                        <div className="col border border-secondary rounded m-4 p-3  tile" style={{display: 'none'}}>
                            <NumberInput label={text.get('channelNumber', lang) + " (1...13)"}
                                min="1" 
                                max="13"
                                step="1"
                                classes={this.state.error} 
                                placeholder={text.get('channelNumber', lang)} 
                                config={this.props.config}
                                changedConfig={this.changedConfig}  
                                value="accessPoint.chnl"  
                            />
                        </div>
                    </div>
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>);
    };
};

export default AccessPoint;