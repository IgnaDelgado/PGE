using System.Text;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Tarea_6
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 

    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        // Funcion Numero de tarjeta valido
        private bool EsNumeroTarjetaValido(string numeroTarjeta)
        {
            // Eliminar espacios y guiones
            numeroTarjeta = numeroTarjeta.Replace(" ", "").Replace("-", "");
            // Verificar que solo contenga dígitos
            if (!Regex.IsMatch(numeroTarjeta, @"^\d+$"))
                return false;
            int suma = 0;
            bool alternar = false;
            // Recorrer los dígitos de derecha a izquierda
            for (int i = numeroTarjeta.Length - 1; i >= 0; i--)
            {
                int digito = int.Parse(numeroTarjeta[i].ToString());
                if (alternar)
                {
                    digito *= 2;
                    if (digito > 9)
                        digito -= 9;
                }
                suma += digito;
                alternar = !alternar;
            }
            // El número es válido si la suma es múltiplo de 10
            return (suma % 10 == 0);
        }

        private void OnlyNumbers(object sender, TextCompositionEventArgs e)
        {
            // Solo permite dígitos 0–9
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);
        }

        // Apretan el boton Boron_Pagar
        private void Boton_Pagar_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrWhiteSpace(TxtNombre.Text) ||
                string.IsNullOrWhiteSpace(TxtCVV.Text) ||
                string.IsNullOrWhiteSpace(TxtApellido.Text) ||
                string.IsNullOrWhiteSpace(TxtEdad.Text) ||
                string.IsNullOrWhiteSpace(TxtTitular.Text) ||
                string.IsNullOrWhiteSpace(TxtNumeroTarjeta.Text) ||
                string.IsNullOrWhiteSpace(TxtVencimiento.Text)
                )
            {   
                MessageBox.Show("Por favor, complete todos los campos.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // Verificar que el numero de la tarjeta sea Luhn valido
            if (!EsNumeroTarjetaValido(TxtNumeroTarjeta.Text) || TxtNumeroTarjeta.Text.Length != 16)
            {
                MessageBox.Show("El número de tarjeta no es válido.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // Verificar CVV
            if(TxtCVV.Text.Length != 3)
            {
                MessageBox.Show("El CVV no es válido.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // Verificar Fecha de vencimiento
            if(!Regex.IsMatch(TxtVencimiento.Text, @"^(0[1-9]|1[0-2])\/\d{2}$"))
            {
                MessageBox.Show("El formato de la fecha de vencimiento no es válido. Use MM/AA.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // Verificar que la tarjeta no este vencida
            var partesFecha = TxtVencimiento.Text.Split('/');
            int mes = int.Parse(partesFecha[0]);
            int año = int.Parse(partesFecha[1]) + 2000; // Asumir siglo 21
            var fechaVencimiento = new DateTime(año, mes, 1).AddMonths(1).AddDays(-1);
            if (fechaVencimiento < DateTime.Now)
            {
                MessageBox.Show("La tarjeta está vencida.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // Si todo es valido, mostrar datos ingresados
            MessageBox.Show(
                $"Nombre: {TxtNombre.Text}\n" +
                $"Apellido: {TxtApellido.Text}\n" +
                $"Edad: {TxtEdad.Text}\n" +
                $"Titular: {TxtTitular.Text}\n" +
                $"Número de Tarjeta: ¨****************\n" +
                $"CVV: ***\n" +
                $"Vencimiento: **/**",
                "Datos Ingresados",
                MessageBoxButton.OK,
                MessageBoxImage.Information
            );

        }
    }
}