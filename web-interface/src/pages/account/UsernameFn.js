import Save from "../Save";
import { closeMenu } from "../../menu/closeMenu";

class UsernameFn extends Save {
    constructor(props) {
        super(props);
        this.state = { menuOpen: 1 };
        
        this.changedConfig = this.changedConfig.bind(this);
    };

    componentDidUpdate() {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
    }
};

export default UsernameFn;