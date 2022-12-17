import Save from "../Save";
import { closeMenu } from "../../menu/closeMenu";

class DisplayFn extends Save {
    constructor(props) {
        super(props);
        
        this.state = {
            menuOpen: 1
        };
        
        this.changedConfig = this.changedConfig.bind(this);
        this.setBrightLimit = this.setBrightLimit.bind(this);
        this.changeAnimType = this.changeAnimType.bind(this);
        this.changeAnimSpeed  = this.changeAnimSpeed.bind(this);
        this.changeClockPoints = this.changeClockPoints.bind(this);
    };

    componentDidUpdate() {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
    }

    setBrightLimit() {
        this.sendGetRequest('brightLimit', 
            `min=${this.props.config.display.brightness.min[this.props.num]}&` +
            `max=${this.props.config.display.brightness.max[this.props.num]}&` +
            `num=${this.props.num}`
        );
    }

    changeAnimType() {
        this.sendGetRequest('animation', 
            `type=${this.props.config.display.animation.type}`
        );
    }

    changeAnimSpeed() {
        this.sendGetRequest('animation', 
            `speed=${this.props.config.display.animation.speed}`
        );
    }

    changeClockPoints() {
        this.sendGetRequest('animation', 
            `points=${this.props.config.display.animation.points}`
        );
    }
};

export default DisplayFn;