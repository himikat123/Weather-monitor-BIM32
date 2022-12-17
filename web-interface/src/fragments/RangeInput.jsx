import Save from "../pages/Save";
import {ReactComponent as ArrowLeft} from "../svg/arrowLeft.svg";
import {ReactComponent as ArrowRight} from "../svg/arrowRight.svg";

class RangeInput extends Save {
    constructor(props) {
        super(props);
        this.changedValue = this.changedValue.bind(this);
        this.minus = this.minus.bind(this);
        this.plus = this.plus.bind(this);
    };

    /**
     * input value has been changed
     * @param {*} event
     */
    changedValue(event) {
        let config = this.setVal(this.props.config, this.props.value, Number(event.target.value));
        this.changedConfig(config);
        if(this.props.callback) this.props.callback();
    }

    /**
     * minus 1
     * @param {*} event
     */
    minus(event) {
        event.preventDefault();
        let val = Number(this.getVal(this.props.config, this.props.value));
        if(val > Number(this.props.limitMin)) val -= Number(this.props.step);
        let config = this.setVal(this.props.config, this.props.value, val);
        this.changedConfig(config);
        if(this.props.callback) this.props.callback();
    }

    /**
     * plus 1
     * @param {*} event
     */
    plus(event) {
        event.preventDefault();
        let val = Number(this.getVal(this.props.config, this.props.value));
        if(val < Number(this.props.limitMax)) val += Number(this.props.step);
        let config = this.setVal(this.props.config, this.props.value, val);
        this.changedConfig(config);
        if(this.props.callback) this.props.callback();
    }

    render() {
        let min = (this.props.limitMin - this.props.min) * 100 / (this.props.max - this.props.min);
        let max = (this.props.max - this.props.limitMax) * 100 / (this.props.max - this.props.min);
        
        return ( 
            <>
                {this.props.label &&
                    <div className="text-center">
                        <label className="form-label mt-3">{this.props.label}</label>
                    </div>
                }
                <div className="d-flex">
                    <div className="me-2 range-a">
                        <a href="#" onClick={this.minus}><ArrowLeft /></a>
                    </div>
                    <div className="range-limit" style={{width: `${min}%`}}><div></div></div>
                    <input type="range" 
                        className="form-range"
                        style={{width: `${100 - max - min}%`}}
                        min={this.props.limitMin}
                        max={this.props.limitMax}
                        step={this.props.step}
                        value={this.getVal(this.props.config, this.props.value)}
                        onChange={this.changedValue}
                    />
                    <div className="range-limit" style={{width: `${max}%`}}><div></div></div>
                    <div className="ms-2 range-a">
                        <a href="#" onClick={this.plus}><ArrowRight /></a>
                    </div>
                    <div className="ms-2 range-val">
                        {Math.round(this.getVal(this.props.config, this.props.value) * 1000) / 1000}{this.props.units}
                    </div>
                </div>
            </>
        );
    };
};

export default RangeInput;