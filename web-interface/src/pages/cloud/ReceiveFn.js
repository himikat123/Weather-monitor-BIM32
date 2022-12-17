import Save from "../Save";
import { closeMenu } from "../../menu/closeMenu";

class ReceiveFn extends Save {
    constructor(props) {
        super(props);
        this.state = {
            menuOpen: 1
        }

        this.changedConfig = this.changedConfig.bind(this);
    }

    /**
     * props have been updated
     * @param {*} nextProps 
     */
    componentDidUpdate() {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
    }
};

export default ReceiveFn;