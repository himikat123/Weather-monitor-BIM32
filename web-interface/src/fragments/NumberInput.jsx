import Save from "../pages/Save";

class NumberInput extends Save {
    constructor(props) {
        super(props);
        this.state = { error: '' }
        this.changedValue = this.changedValue.bind(this);
    };

    /**
     * validate the entered channel number
     * @param {*} event 
     */
    validate(event){
        let val = event.target.value;
        if(isNaN(val) || val < Number(this.props.min) || val > Number(this.props.max)) {
            this.setState({error: ' bg-danger'});
        }
        else this.setState({error: ''});
    }

    /**
     * input value has been changed
     * @param {*} newValue
     */
    changedValue(event) {
        this.validate(event);
        let config = this.setVal(this.props.config, this.props.value, Number(event.target.value));
        this.changedConfig(config);
    }

    render(){
        return ( 
            <>
                <label className="form-label mt-3">
                    {this.props.label}
                </label>
                <input type="number" 
                    className={this.state.error + ' form-control ' + this.props.classes}
                    min={this.props.min}
                    max={this.props.max}
                    step={this.props.step}
                    placeholder={this.props.placeholder}
                    value={this.getVal(this.props.config, this.props.value)}
                    onChange={this.changedValue}
                />
            </>
        );
    };
};

export default NumberInput;