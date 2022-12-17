import UsernameFn from "./UsernameFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import TextInput from "../../fragments/TextInput";
import SwitchInput from "../../fragments/SwitchInput";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Username extends UsernameFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        return (<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('username', lang)} />

                    {this.props.config.hasOwnProperty('account') && 
                    <div className="d-flex justify-content-center">
                        <div className="row">

                        {/* Username */}
                        <div className="col-12 col-md-6 d-flex justify-content-center">
                            <div className="border border-secondary rounded m-2 p-3 account w-100">
                                <TextInput maxLength="32" 
                                    placeholder={text.get('username', lang)}
                                    label={text.get('username', lang)} 
                                    value="account.name" 
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig} 
                                />
                            </div>
                        </div>

                        {/* Login require */}
                        <div className="col-12 col-md-6 d-flex justify-content-center">
                            <div className="border border-secondary rounded m-2 p-3 account w-100">
                                <SwitchInput value="account.required"
                                    label={text.get('requireUsernameToLogin', lang)}
                                    config={this.props.config} 
                                    changedConfig={this.changedConfig}
                                />
                            </div>
                        </div></div>
                    </div>}
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>);
    };
};

export default Username;