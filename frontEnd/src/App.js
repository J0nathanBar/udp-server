import logo from './logo.svg';
import './App.css';
import {TextForm} from './components'
import { Reciever,RecieverJs,Transmitter,TransmitterJs,TransmitterApi } from './containers';
import Button from "react"
function App() {
  return (
    <div className="counter">
     
    <h1>Reciever</h1>
    <TextForm name="RecvConf.json"/>
    <h1>Transmitter</h1>
     <TextForm name = "TransConf.json"/>
    </div>
    
  );
}

export default App;
