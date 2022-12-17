import Save from "../pages/Save";

class SelectInput extends Save {
    constructor(props) {
        super(props);
        this.props.changedConfig(this.props.config);
        this.changedValue = this.changedValue.bind(this);
    };

    /**
     * select value has been changed
     * @param {*} event
     */
    changedValue(event) {
        let config = this.setVal(this.props.config, this.props.value, Number(event.target.value));
        this.changedConfig(config);
        if(this.props.callback) this.props.callback();
    }

    render(){
        return(
            <>
                <label className="form-label mt-3">
                    {this.props.label}
                </label>
                <select className="form-select" 
                    value={this.getVal(this.props.config, this.props.value)}
                    onChange={this.changedValue}
                >
                    {this.props.options.map((text, val) => {
                        return <option key={val} value={val}>{text}</option>
                    })}
                </select>
            </>
        );
    };
};

export default SelectInput;