import React from "react";
import Languages from "../Languages";

class ModalFileViewer extends React.Component{
    render(){
        let text = new Languages();
        let lang = this.props.lang;

        return(
            <div className="modal fade" id="fileviewer" tabIndex="-1">
                <div className="modal-dialog modal-dialog-scrollable modal-xl">
                    <div className="modal-content">
                        <div className="modal-header">
                            <h5 className="modal-title">
                                {this.props.fileName}
                            </h5>
                            <button type="button" className="btn-close" data-bs-dismiss="modal" title={text.get('close', lang)}></button>
                        </div>
                        <div className="modal-body">
                            <pre>{this.props.data}</pre>
                        </div>
                        <div className="modal-footer">
                            <button type="button" id="closeFileViewer" className="btn btn-secondary" data-bs-dismiss="modal">
                                {text.get('close', lang)}
                            </button>
                        </div>
                    </div>
                </div>
            </div>
        );
    };
};

export default ModalFileViewer;