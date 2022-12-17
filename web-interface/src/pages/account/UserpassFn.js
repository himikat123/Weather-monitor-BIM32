import Save from "../Save";
import { closeMenu } from "../../menu/closeMenu";

class UserpassFn extends Save {
    constructor(props) {
        super(props);
        this.state = {
            oldPass: "",
            newPass: "", 
            menuOpen: 1 
        };
        
        this.changedConfig = this.changedConfig.bind(this);
        this.oldPass = this.oldPass.bind(this);
        this.newPass = this.newPass.bind(this);
    };

    componentDidUpdate() {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
    }

    /**
     * old password has been changed
     * @param {*} pass 
     */
    oldPass(pass) {
        this.setState({ oldPass: pass }, () => {
            this.props.pass(this.state.oldPass, this.state.newPass);
        });
    }

    /**
     * new password has been changed
     * @param {*} pass 
     */
    newPass(pass) {
        this.setState({ newPass: pass }, () => {
            this.props.pass(this.state.oldPass, this.state.newPass);
        });
    }
};

export default UserpassFn;