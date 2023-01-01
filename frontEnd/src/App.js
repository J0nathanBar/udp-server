import logo from './logo.svg';
import './App.css';
import { Reciever, Transmitter, FileForm } from './containers';
function App() {
  return (
    <div className="counter">
      <h1>Virtual Channel</h1>

     <Transmitter/>
     <Reciever/>
     <FileForm/>


    </div>

  );
}

export default App;
