import React from "react";

function PageHeader(props) {
    const className = 'd-flex justify-content-center';
    return(
        <div className="row">
            <div className="col m-2 p-2">
                {props.h == 1 && <h1 className={className}>{props.text}</h1>}
                {props.h == 2 && <h2 className={className}>{props.text}</h2>}
                {props.h == 3 && <h3 className={className}>{props.text}</h3>}
                {props.h == 4 && <h4 className={className}>{props.text}</h4>}
                {props.h == 5 && <h5 className={className}>{props.text}</h5>}
                {props.h == 6 && <h6 className={className}>{props.text}</h6>}
            </div>
        </div>
    );
}

export default PageHeader;