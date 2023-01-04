import logo from './logo.svg';
import './App.css';
import { Reciever, Transmitter} from './containers';
function App() {
  return (
    <div className="counter">
      <h1>Virtual Channel</h1>
      <h2>Transmitter</h2>
    <Transmitter/>
    <h2>Reciever</h2>
    <Reciever/>
  
   


    </div>

  );
}

export default App;
