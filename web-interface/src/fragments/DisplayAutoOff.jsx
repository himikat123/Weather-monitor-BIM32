import Save from "../pages/Save";
import Languages from "../Languages";
import NumberInput from "./NumberInput";

class DisplayAutoOff extends Save {
    constructor(props) {
        super(props);
        this.changedConfig = this.changedConfig.bind(this);
        this.displayOnOff = this.displayOnOff.bind(this);
    }

    /**
     * 
     */
    displayOnOff() {
        this.sendGetRequest('dispToggle', 
            `num=${this.props.num}`
        );
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
                </div>
            </div>
        )
    }
};

export default DisplayAutoOff;