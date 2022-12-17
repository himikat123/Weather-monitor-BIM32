import Save from "../Save";
import Languages from "../../Languages";
import { closeMenu } from "../../menu/closeMenu";

class WeatherFn extends Save {
    constructor(props) {
        super(props);
        this.state = {
            provider: ['http://openweathermap.org', 'http://weatherbit.io'],
            color: 'text-info',
            temperature: '--',
            humidity: '--',
            pressure: '--',
            wind: '--',
            dir: 0,
            description: '--',
            city: '--',
            country: '',
            latitude: '--',
            longitude: '--',
            checkWeather: '',
            getCoordinates: '',
            menuOpen: 1
        };

        this.changedConfig = this.changedConfig.bind(this);
        this.getGeolocation = this.getGeolocation.bind(this);
        this.getWeather = this.getWeather.bind(this);
    };

    /**
     * Get Geolocation from browser
     */
    getGeolocation() {
        this.setState({getCoordinates: 'spinner-border spinner-border-sm ms-2'});
        if(navigator.geolocation) {
            navigator.geolocation.getCurrentPosition((position) => {
                this.setState({getCoordinates: ''});
                this.props.config.weather.lat = position.coords.latitude;
                this.props.config.weather.lon = position.coords.longitude;
                this.changedConfig(this.props.config);
            });
        }
    }

    /**
     * Get weather forecast from internet
     */
    getWeather() {
        this.setState({checkWeather: 'spinner-border spinner-border-sm ms-2'});
        let current = '', citysearch = '';
        let text = new Languages();
        let lang = this.props.config.lang;
        
        // from openweathermap.org
        if(this.props.config.weather.provider == 0) {
            current = "https://api.openweathermap.org/data/2.5/weather";
            if(this.props.config.weather.citysearch == 0) citysearch = `?q=${this.props.config.weather.city}`;
            if(this.props.config.weather.citysearch == 1) citysearch = `?id=${this.props.config.weather.cityid}`;
            if(this.props.config.weather.citysearch == 2) citysearch = `?lat=${this.props.config.weather.lat}&lon=${this.props.config.weather.lon}`;
            current += `${citysearch}&units=metric&appid=${this.props.config.weather.appid[0]}`;
            current += `&lang=${this.props.config.lang}`;
        }

        // from weatherbit.io
        if(this.props.config.weather.provider == 1) {
            current = `https://api.weatherbit.io/v2.0/current?key=${this.props.config.weather.appid[1]}`;
            if(this.props.config.weather.citysearch == 0) citysearch = `&city=${this.props.config.weather.city}`;
            if(this.props.config.weather.citysearch == 1) citysearch = `&city_id=${this.props.config.weather.cityid}`;
            if(this.props.config.weather.citysearch == 2) citysearch = `&lat=${this.props.config.weather.lat}&lon=${this.props.config.weather.lon}`;
            current += `${citysearch}`;
            current += `&lang=${this.props.config.lang}`;
        }
        console.log(current);
        fetch(current).then((response) => {
            return response.json();
        }).then((json) => {
            this.setState({checkWeather: ''});
            try {
                // get data from openweathermap.org
                if(this.props.config.weather.provider == 0) {
                    this.setState({
                        color: 'text-info',
                        temperature: json["main"]["temp"],
                        humidity: json["main"]["humidity"],
                        pressure: Math.round(json["main"]["pressure"] * 0.75),
                        wind: json["wind"]["speed"],
                        dir: json["wind"]["deg"],
                        description: json["weather"][0]["description"],
                        city: json["name"],
                        country: json["sys"]["country"],
                        latitude: json["coord"]["lat"],
                        longitude: json["coord"]["lon"]
                    });
                }
                // get data from weatherbit.io
                if(this.props.config.weather.provider == 1) {
                    this.setState({
                        color: 'text-info',
                        temperature: json["data"][0]["temp"],
                        humidity: Math.round(json["data"][0]["rh"]),
                        pressure: Math.round(json["data"][0]["pres"] * 0.75),
                        wind: (json["data"][0]["wind_spd"]).toFixed(1),
                        dir: (json["data"][0]["wind_dir"]),
                        description: json["data"][0]["weather"]["description"],
                        city: json["data"][0]["city_name"],
                        country: json["data"][0]["country_code"],
                        latitude: json["data"][0]["lat"],
                        longitude: json["data"][0]["lon"]
                    });
                }
            }
            catch(e) {
                this.setState({
                    color: 'text-danger',
                    temperature: '--',
                    humidity: '--',
                    pressure: '--',
                    wind: '--',
                    dir: 0,
                    description: text.get('weatherCheckError', lang),
                    city: '--',
                    country: '',
                    latitude: '--',
                    longitude: '--'
                });
            }
        });
    }

    componentDidUpdate() {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
    }
};

export default WeatherFn;