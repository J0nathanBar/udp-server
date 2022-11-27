import logo from './logo.svg';
import './App.css';
import {TextForm} from './components'
import { Reciever,RecieverJs,Transmitter,TransmitterJs,TransmitterApi } from './containers';
import Button from "react"
function App() {
  return (
    <div className="counter">
     
    <Reciever/>
     <TextForm />
    </div>
    
  );
}

export default App;
