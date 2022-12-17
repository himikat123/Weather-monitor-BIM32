import DisplayFn from "./DisplayFn1";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import SelectInput from "../../fragments/SelectInput";
import DisplayAutoOff from "../../fragments/DisplayAutoOff";
import DisplayBrightness from "../../fragments/DisplayBrightness";
import Display1temp from "../../fragments/Display1temp";
import Display1hum from "../../fragments/Display1hum";
import Display1pres from "../../fragments/Display1pres";
import Display1volt from "../../fragments/Display1volt";
import Display1bat from "../../fragments/Display1bat";
import Display1descr from "../../fragments/Display1descr";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Display extends DisplayFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        return (<>
            <Menu language={lang} />
            
            <div className="d-flex justify-content-center">
                {this.props.config.hasOwnProperty('display') &&
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('display', lang) + ' ' + (Number(this.props.num) + 1)} />
            
                    <div className="row justify-content-center">

                        {/* Display type */}
                        <div className="col border border-secondary rounded m-4 pb-3 disp-card">
                            <SelectInput value={`display.type.${this.props.num}`}
                                label={text.get('displayType', lang)}
                                options={["NX4832K035", "NX4832T035"]} 
                                config={this.props.config} 
                                changedConfig={this.changedConfig}
                            />
                            <div className="mt-4">
                                {text.get('maximumDisplayCurrent', lang)}: <span className="indication">145{text.get('ma', lang)}</span>
                            </div>
                        </div>
                        
                        {/* Brightness */}
                        <DisplayBrightness config={this.props.config} data={this.props.data} num="0" changedConfig={this.changedConfig} />

                        {/* Display auto-off */}
                        <DisplayAutoOff config={this.props.config} num="0" changedConfig={this.changedConfig} />
                    </div>
                
                    <div className="row justify-content-center">
                        
                        {/* Temperature inside */}
                        <Display1temp place="In" config={this.props.config} changedConfig={this.changedConfig} />
                        
                        {/* Humidity inside */}
                        <Display1hum place="In" config={this.props.config} changedConfig={this.changedConfig} />
                        
                        {/* Battery voltage */}
                        <Display1volt config={this.props.config} changedConfig={this.changedConfig} />

                        {/* Battery symbol */}
                        <Display1bat config={this.props.config} changedConfig={this.changedConfig} />

                        {/* Additional description */}
                        <Display1descr config={this.props.config} changedConfig={this.changedConfig} />
                        
                        {/* Temperature outside */}
                        <Display1temp place="Out" config={this.props.config} changedConfig={this.changedConfig} />
                        
                        {/* Humidity outside */}
                        <Display1hum place="Out" config={this.props.config} changedConfig={this.changedConfig} />
                        
                        {/* Pressure outside */}
                        <Display1pres config={this.props.config} changedConfig={this.changedConfig} />
                    </div>
                </div>}
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" />
        </>);
    };
}

export default Display;