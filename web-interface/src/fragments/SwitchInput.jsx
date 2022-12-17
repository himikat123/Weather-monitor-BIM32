import Save from "../pages/Save";

class SwitchInput extends Save{
    constructor(props){
        super(props);
        this.changedValue = this.changedValue.bind(this);
    };

    /**
     * input value has been changed
     * @param {*} event
     */
    changedValue(event){
        let config = this.setVal(this.props.config, this.props.value, event.target.checked ? 1 : 0);
        this.changedConfig(config);
    }

    render(){
        return (
            <div className="form-check form-switch">
                <input id={this.props.value} 
                    className="form-check-input" 
                    type="checkbox" 
                    checked={this.getVal(this.props.config, this.props.value) == 1 ? true : false}
                    onChange={this.changedValue}
                />
                <label htmlFor={this.props.value} 
                    className="form-label">
                    {this.props.label}
                </label>
            </div> 
        );
    };
};

export default SwitchInput;