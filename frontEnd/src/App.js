import './App.css';


import { HomePage, Reciever, Transmitter, Manual, Contact } from './containers';
import { BrowserRouter as Router, Route,Routes } from 'react-router-dom';

function App() {


  return (
    <div className='bdy'>
    <Router >
      <Routes>
        <Route path='/' element={<HomePage />} />
        <Route path='/Transmitter' element={<Transmitter />} />
        <Route path='/Reciever' element={<Reciever />} />
        <Route path='/Manual' element={<Manual />} />
        <Route path='/Contact' element={<Contact />} />



      </Routes>
    </Router>
    </div>

  );
}
export default App;
