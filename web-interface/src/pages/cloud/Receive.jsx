import ReceiveFn from "./ReceiveFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import SwitchInput from "../../fragments/SwitchInput";
import NumberInput from "../../fragments/NumberInput";
import TextInput from "../../fragments/TextInput";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Receive extends ReceiveFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;
        
        return (<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('dataReceive', lang)} />

                    {this.props.config.hasOwnProperty('thingspeakReceive') &&
                    <div className="row justify-content-center">
                        
                        {/* Receive from thingspeak.com */}
                        <div className="col border border-secondary rounded m-4 p-3">

                            {/* on/off switch */}
                            <SwitchInput label={text.get('receiveDataFrom', lang) + ' thingspeak.com'}
                                value={'thingspeakReceive.turnOn'}
                                config={this.props.config} 
                                changedConfig={this.changedConfig}
                            />

                            <div className="row">

                                {/* Period */}
                                <div className="col-12 col-sm-6 col-lg-3 mt-3">
                                    <NumberInput label={text.get('periodMinutes', lang)}
                                        value={'thingspeakReceive.period'} 
                                        min="1"
                                        max="999"
                                        step="1"
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig}    
                                    />
                                </div>

                                {/* Channel ID */}
                                <div className="col-12 col-sm-6 col-lg-3 mt-3">
                                    <TextInput label="Channel ID"
                                        maxLength="20" 
                                        placeholder="Channel ID" 
                                        value="thingspeakReceive.channelID" 
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig} 
                                    />
                                </div>
                                
                                {/* Read API Key */}
                                <div className="col-12 col-sm-6 col-lg-3 mt-3">
                                    <TextInput label="Read API Key"
                                        maxLength="32" 
                                        placeholder="Read API Key" 
                                        value="thingspeakReceive.rdkey" 
                                        config={this.props.config} 
                                        changedConfig={this.changedConfig} 
                                    />
                                </div>

                                {/* Data expiration time */}
                                <div className="col-12 col-sm-6 col-lg-3 mt-3">
                                    <NumberInput min="1" max="100" step="1" 
                                        placeholder={text.get('dataExpirationTime', lang)}
                                        label={text.get('dataExpirationTime', lang)}
                                        value={'thingspeakReceive.expire'} 
                                        config={this.props.config} changedConfig={this.changedConfig} 
                                    />
                                </div>
                            </div>
                        </div>
                    </div>}
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>);
    }
};

export default Receive;