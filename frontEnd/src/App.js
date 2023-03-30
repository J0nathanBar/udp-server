import logo from './logo.svg';
import './App.css';


import { HomePage, Reciever, Transmitter } from './containers';
import { BrowserRouter as Router, Route, Link,Routes } from 'react-router-dom';

function App() {

  return (
    <div className="counter">
      <Routes>
    
          <Route path="/" element={<HomePage />} />
          <Route path="/Transmitter" element={<Transmitter />} />
      
      </Routes>
    </div>

  );
}
export default App;
