import React from "react";
import Languages from "../Languages";

class ModalRestart extends React.Component{
    /**
     * send restart command
     */
    sendRestart(){
        document.querySelector('#closeRestartModal').click();
        let origin = window.location.origin;
        let href = [origin.split(':')[0], origin.split(':')[1]].join(':');
        fetch(`${href}/esp/restart`, { mode: 'cors' })
    }

    render(){
        let text = new Languages();
        let lang = this.props.lang;

        return(
            <div className="modal fade" id="modalRestart" tabIndex="-1">
                <div className="modal-dialog">
                    <div className="modal-content">
                        <div className="modal-header">
                            <h5 className="modal-title">{text.get('confirmation', lang)}</h5>
                            <button id="closeRestartModal" type="button" className="btn-close" data-bs-dismiss="modal" title={text.get('close', lang)}></button>
                        </div>
                        <div className="modal-body">{text.get('restartConfirmation', lang)}</div>
                        <div className="modal-footer">
                            <button type="button" className="btn btn-danger" onClick={this.sendRestart}>{text.get('restart', lang)}</button>
                            <button type="button" className="btn btn-secondary" data-bs-dismiss="modal">{text.get('cancel', lang)}</button>
                        </div>
                    </div>
                </div>
            </div>
        );
    };
};
export default ModalRestart;