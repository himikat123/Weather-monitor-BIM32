import Save from "../Save";
import { closeMenu } from "../../menu/closeMenu";

class AccessPointFn extends Save{
    constructor(props){
        super(props);
        this.state = { data: {}, error: '', menuOpen: 1 };
        this.changedConfig = this.changedConfig.bind(this);
    };

    /**
     * props have been updated
     * @param {*} nextProps 
     */
    componentDidUpdate(nextProps){
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
        if(this.state.data != nextProps.data) this.setState({data: nextProps.data});
    }
};

export default AccessPointFn;