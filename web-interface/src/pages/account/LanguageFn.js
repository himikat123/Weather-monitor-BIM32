import Save from "../Save";
import { closeMenu } from "../../menu/closeMenu";

class LanguageFn extends Save {
    constructor(props) {
        super(props);
        this.state = { menuOpen: 1 };
        
        this.changedConfig = this.changedConfig.bind(this);
        this.changedLang = this.changedLang.bind(this);
    };

    componentDidUpdate() {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
    }

    changedLang(event) {
        this.props.config.lang = event.target.value;
        this.changedConfig(this.props.config);
    }
};

export default LanguageFn;