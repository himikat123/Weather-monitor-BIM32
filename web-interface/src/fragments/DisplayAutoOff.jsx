import Save from "../pages/Save";
import Languages from "../Languages";
import NumberInput from "./NumberInput";
import SwitchInput from "./SwitchInput";

class DisplayAutoOff extends Save {
    constructor(props) {
        super(props);
        this.changedConfig = this.changedConfig.bind(this);
        this.displayOnOff = this.displayOnOff.bind(this);
        this.changedHrFrom = this.changedHrFrom.bind(this);
        this.changedHrTo = this.changedHrTo.bind(this);
    }

    /**
     * 
     */
    displayOnOff() {
        this.sendGetRequest('dispToggle', 
            `num=${this.props.num}`
        );
    }

    /**
     * hourly time "from" has been changed
     * @param {*} event 
     */
    changedHrFrom(event) {
        this.props.config.display.nightOff.from[this.props.num] = Number(event.target.value.split(':')[0]);
        this.changedConfig(this.props.config);
    }

    /**
     * hourly time "to" has been changed
     * @param {*} event 
     */
    changedHrTo(event) {
        this.props.config.display.nightOff.to[this.props.num] = Number(event.target.value.split(':')[0]);
        this.changedConfig(this.props.config);
    }
    
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        return (
            <div className="col border border-secondary rounded m-4 pb-3 disp-card">
                <NumberInput min="0" 
                    max="1440" 
                    step="1" 
                    placeholder={text.get('minutes', lang)}
                    label={text.get('turnOffDisplayWhenIdle', lang)}
                    value={`display.autoOff.${this.props.num}`}
                    config={this.props.config} 
                    changedConfig={this.changedConfig} 
                />
                <label className="form-label mt-2">
                    (0 - {text.get('neverTurnOff', lang)})
                </label>

                <div className="d-flex justify-content-center my-3">
                    <button className="btn btn-success mt-3 px-4" onClick={this.displayOnOff}>
                        {text.get('turnOnOffNow', lang)}
                    </button>
                </div><hr />

                <SwitchInput value={`display.nightOff.need.${this.props.num}`}
                    label={text.get('turnOffDisplayAtNight', lang)}
                    config={this.props.config} 
                    changedConfig={this.changedConfig}
                />
                <div className="text-center">
                    <label className="form-label mx-2 mt-4">
                        {text.get('from', lang)}
                    </label>
                    <input type="time" 
                        className="mt-4 time-control" 
                        step="3600" 
                        value={
                            (this.props.config.display.nightOff.from[this.props.num] < 10 ? 
                            '0' + this.props.config.display.nightOff.from[this.props.num] : 
                            this.props.config.display.nightOff.from[this.props.num]
                            ) + ':00'}
                            onChange={this.changedHrFrom} 
                    />
                    <label className="form-label mx-2 mt-4">
                        {text.get('to', lang)}
                    </label>
                    <input type="time" 
                        className="mt-4 time-control" 
                        step="3600"
                        value={
                            (this.props.config.display.nightOff.to[this.props.num] < 10 ? 
                            '0' + this.props.config.display.nightOff.to[this.props.num] : 
                            this.props.config.display.nightOff.to[this.props.num]
                            ) + ':00'}
                            onChange={this.changedHrTo} 
                    />
                </div>
            </div>
        )
    }
};

export default DisplayAutoOff;