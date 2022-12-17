import UserpassFn from "./UserpassFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import PasswordField from "../../fragments/PasswordField";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Userpass extends UserpassFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        return (<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('changePassword', lang)} />

                    <div className="row d-flex justify-content-center">

                        <div className="col border border-secondary rounded m-4 pb-4 account">
                            
                            {/* Old password */}
                            <PasswordField label={text.get('oldPassword', lang)}
                                num="oldPass" 
                                config={this.props.config} 
                                changedConfig={this.changedConfig}
                                oldPass={this.oldPass}
                            />

                            {/* New password */}
                            <PasswordField label={text.get('newPassword', lang)}
                                num="newPass" 
                                config={this.props.config} 
                                changedConfig={this.changedConfig} 
                                newPass={this.newPass}
                            />
                        </div>
                    </div>
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" pass="1" />
        </>);
    };
};

export default Userpass;