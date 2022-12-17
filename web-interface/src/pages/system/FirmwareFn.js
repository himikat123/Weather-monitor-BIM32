import Save from "../Save";
import { closeMenu } from "../../menu/closeMenu";
import axios from 'axios';

class FirmwareFn extends Save {
    constructor(props) {
        super(props);
        this.state = { 
            menuOpen: 1,
            percentage: 0
        }
        this.firmwareUpdate = this.firmwareUpdate.bind(this);
    }

    /**
    * Props have been updated 
    */
    componentDidUpdate() {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
    }

    /**
     * Send firmware file to ESP32
     */
    async firmwareUpdate(event) {
        event.preventDefault();
        let formData = new FormData();
        formData.append("file", document.querySelector('#upload_form')[0].files[0]);

        const onUploadProgress = (event) => {
            const percentage = Math.round((100 * event.loaded) / event.total);
            this.setState({ percentage: percentage });
        };

        try {
            let origin = window.location.origin;
            let href = [origin.split(':')[0], origin.split(':')[1]].join(':');
            await axios.post(`${href}/esp/fwUpdate`, formData, {
                headers: {
                    "Content-Type": "multipart/form-data",
                }, 
                onUploadProgress
            });
        } catch (error) { }
    }
};

export default FirmwareFn;