using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Tarea_5
{
    public partial class Form1 : Form
    {
        private object dateTimePicker1;

        public Form1()
        {
            InitializeComponent();
            textBox3.KeyPress += textBox3_KeyPress; // <- ENGANCHADO
            textBox4.KeyPress += textBox4_KeyPress; // <- ENGANCHADO
            textBox5.KeyPress += textBox5_KeyPress; // <- ENGANCHADO

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }


        private void textBox3_KeyPress(object sender, KeyPressEventArgs e)
        {
            // Permitir solo dígitos y la tecla de retroceso
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar))
            {
                e.Handled = true; // Bloquea la tecla
            }
        }

        private void textBox4_KeyPress(object sender, KeyPressEventArgs e)
        {
            // Permitir solo dígitos y la tecla de retroceso
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar))
            {
                e.Handled = true; // Bloquea la tecla
            }
        }

        private void textBox5_KeyPress(object sender, KeyPressEventArgs e)
        {
            // Permitir solo dígitos y la tecla de retroceso
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar))
            {
                e.Handled = true; // Bloquea la tecla
            }
        }
        private void button1_Click(object sender, EventArgs e)
        {
            using (var form2 = new Form2())
            {
                // Abro Form2 como diálogo modal
                if (form2.ShowDialog(this) == DialogResult.OK)
                {
                    // Cuando se cierra con Confirmar, leo el asiento elegido
                    label13.Text = "Asiento seleccionado: " + form2.AsientoElegido;
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            // Verificar que los campos obligatorios no estén vacíos
            
            if (string.IsNullOrWhiteSpace(textBox1.Text) ||
                string.IsNullOrWhiteSpace(textBox2.Text) ||
                string.IsNullOrWhiteSpace(textBox3.Text) ||
                string.IsNullOrWhiteSpace(textBox4.Text) ||
                string.IsNullOrWhiteSpace(textBox5.Text)
                )
            {
                MessageBox.Show("Por favor, complete todos los campos obligatorios.", "Campos incompletos", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return; // Salir del método si hay campos vacíos}
            }
            
            if (textBox4.Text.Length != 16)
            {
                MessageBox.Show("El número de tarjeta debe tener 16 dígitos.", "Número de tarjeta inválido", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }

            if(textBox5.Text.Length != 3)
            {
                MessageBox.Show("El código de seguridad debe tener 3 dígitos.", "Código de seguridad inválido", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }

            // Verificar que la tarjeta cumpla Luhn
            int sum = 0;
            bool alternate = false;
            for (int i = textBox4.Text.Length - 1; i >= 0; i--)
            {
                int n = int.Parse(textBox4.Text[i].ToString());
                if (alternate)
                {
                    n *= 2;
                    if (n > 9)
                        n -= 9;
                }
                sum += n;
                alternate = !alternate;
            }
            if (sum % 10 != 0)
            {
                MessageBox.Show("El número de tarjeta no es válido según el algoritmo de Luhn.", "Número de tarjeta inválido", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return; // Salir del método si la tarjeta no es válida
            }

            // Verificar que el vencimiento no esté en el pasado
            if (dateTimePicker2.Value.Date < DateTime.Now.Date)
            {
                MessageBox.Show("La fecha de vencimiento no puede estar en el pasado.",
                                "Fecha de vencimiento inválida",
                                MessageBoxButtons.OK,
                                MessageBoxIcon.Warning);
                return;
            }




            // Mostrar datos del pasajero
            MessageBox.Show($"Datos del pasajero:\n\n" +
                $"Nombre: {textBox1.Text}\n" +
                $"Apellido: {textBox2.Text}\n" +
                $"DNI: {textBox3.Text}\n" +
                $"Número de tarjeta: {textBox4.Text}\n" +
                $"Código de seguridad: {textBox5.Text}\n\n" +
                $"{label13.Text}", "Confirmación de datos", MessageBoxButtons.OK, MessageBoxIcon.Information);



        }
    }
}
