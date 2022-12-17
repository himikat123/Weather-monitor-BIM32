import Save from "../pages/Save";

class ColorInput extends Save {
    constructor(props) {
        super(props);
        this.changedValue = this.changedValue.bind(this);
        this.setNewColor = this.setNewColor.bind(this);
    };

    /**
     * input value has been changed
     * @param {*} event
     */
    changedValue(event) {
        let config = this.setVal(this.props.config, this.props.value, event.target.value);
        this.changedConfig(config);
        this.setNewColor();
    }

    setNewColor() {
        let color = this.getVal(this.props.config, this.props.value).replace('#','');
        this.sendGetRequest('color', 'hex=' + color + '&num=' + this.props.num);
    }

    render() {
        return (
            <>
                <label className="form-label mt-3">
                    {this.props.label}
                </label>
                <input type="color" 
                    className="form-control form-control-color w-100"
                    value={this.getVal(this.props.config, this.props.value)}
                    onChange={this.changedValue}
                />
            </>
        );
    };
};

export default ColorInput;