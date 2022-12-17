import DefaultFn from "./DefaultFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";

class Default extends DefaultFn {
    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        return (<>
            <Menu language={lang} /> 
            
            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('defaultSettings', lang)} />
            
                    <div className="row justify-content-center">
                        <div className="col border border-secondary rounded m-4 p-3 backup-card">
                            <div className="accordion" id="jsonDefault">
                                <div className="accordion-item">
                                    <h2 className="accordion-header" id="headingDefault">
                                        <button className="accordion-button" 
                                            type="button" 
                                            data-bs-toggle="collapse" 
                                            data-bs-target="#configDefault"
                                        >
                                            {text.get('fileContents', lang)}
                                        </button>
                                    </h2>
                                    <div id="configDefault" 
                                        className="accordion-collapse collapse configCode"
                                        data-bs-parent="#jsonConfig"
                                    >
                                        <div className="accordion-body">
                                            <pre>{JSON.stringify(this.state.config, null, '\t')}</pre>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>

                    <div className="text-center">
                        <button className={"btn mt-3 " + this.state.saveButtonColor} 
                            onClick={this.reset}
                        >
                            {text.get('resetToFactory', lang)}
                        </button>
                    </div>
                </div>
            </div>

            <SaveRestart config={this.props.config} restart="1" />
        </>);
    }
};

export default Default;