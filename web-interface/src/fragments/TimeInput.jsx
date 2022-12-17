import Save from "../pages/Save";

class TextInput extends Save {
    constructor(props) {
        super(props);
        this.changedValue = this.changedValue.bind(this);
    };

    /**
     * input value has been changed
     * @param {*} event
     */
    changedValue(event) {
        let config = this.setVal(this.props.config, this.props.value, event.target.value);
        this.changedConfig(config);
    }

    render(){
        return ( 
            <>
                <label className="form-label mt-3">
                    {this.props.label}
                </label>
                <input type="time" 
                    className="form-control"
                    placeholder={this.props.placeholder}
                    value={this.getVal(this.props.config, this.props.value)}
                    onChange={this.changedValue}
                />
            </>
        );
    };
};

export default TextInput;