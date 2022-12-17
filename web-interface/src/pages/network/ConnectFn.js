import Save from "../Save";
import { closeMenu } from "../../menu/closeMenu";

class ConnectFn extends Save {
    constructor(props) {
        super(props);
        this.state = { data: {}, menuOpen: 1 };
        this.changedConfig = this.changedConfig.bind(this);
        this.changedType = this.changedType.bind(this);
    };

    /**
     * props have been updated
     * @param {*} nextProps 
     */
    componentDidUpdate(nextProps) {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
        if(this.state.data != nextProps.data) this.setState({ data: nextProps.data });
    }

    /**
     * network type has been changed
     * @param {*} newConfig 
     */
    changedType(event) {
        this.props.config.type = event.target.value;
        this.changedConfig(this.props.config);
    }
};

export default ConnectFn;