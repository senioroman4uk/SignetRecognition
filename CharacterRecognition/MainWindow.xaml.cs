using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Win32;
using Wrapper;

namespace CharacterRecognition
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private RecogniserWrapper recogniser;
        private DataWraper dataWraper;
        public MainWindow()
        {
            InitializeComponent();
            try
            {
                recogniser = new RecogniserWrapper("C:\\results", "C:\\templates");
            }
            catch (Exception)
            {
                MessageBox.Show("Не знайдено папку для шаблонів, робота програми буде припинена", "Помилка",
                        MessageBoxButton.OK, MessageBoxImage.Error);
                Environment.Exit(0);
            }
            //Прив'язую об'єкт з даними до форми
            dataWraper = new DataWraper();
            DataContext = dataWraper;

        }

        private void DocumentBorder_OnMouseDown(object sender, MouseButtonEventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog { Filter = "(.jpg)|*.jpg" };
            var dialogResult = dialog.ShowDialog();
            if (dialogResult == true)
            {
                try
                {
                    var uri = new Uri(dialog.FileName);
                    Document.Source = new BitmapImage(uri);
                    DocumentBorder.BorderThickness = new Thickness(0);
                    DocumentBorder.Background = Brushes.White;
                    dataWraper.Filename = System.IO.Path.GetFileName(dialog.FileName);
                    dataWraper.Filetype = System.IO.Path.GetExtension(dialog.FileName);
                    FileInfo info = new FileInfo(dialog.FileName);
                    dataWraper.Filesize = (info.Length/1024).ToString();
                    if (recogniser.findCircle(1, new[] {dialog.FileName}) != 0)
                    {
                        MessageBox.Show("Помилка при розпізнаванні печатки", "Помилка",
                        MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    }

                    long distanse = Int64.MaxValue;
                    string template = "";
                    var files = Directory.GetFiles(recogniser.TemplatesPath, "*.jpg");
                    if (files.Length == 0)
                    {
                        MessageBox.Show("У папці з шаблонами не знайдено жодного підходящого файла", "Помилка",
                        MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    }

                    //Розраховую відстань від кожного шаблона до знайденої печатки
                    foreach (var file in files)
                    {
                        var current = recogniser.pictureDiffHash(file, recogniser.ResultsPath + "signet.jpg");
                        if (current < distanse)
                        {
                            distanse = current;
                            template = file;
                        }
                    }

                    var bitmap = new BitmapImage();
                    
                    //Обновляю печатку у вікні
                    bitmap.BeginInit();
                    bitmap.CacheOption = BitmapCacheOption.None;
                    bitmap.CacheOption = BitmapCacheOption.OnLoad;
                    bitmap.CreateOptions = BitmapCreateOptions.IgnoreImageCache;
                    bitmap.UriSource = new Uri(recogniser.ResultsPath + "signet.jpg");
                    bitmap.EndInit();
                    Result.Source = bitmap;
                    
                    ResultBorder.BorderThickness = new Thickness(0);
                    ResultBorder.Background = Brushes.White;
                    uri = new Uri(template);
                    Standard.Source = new BitmapImage(uri);
                    StandardBorder.BorderThickness = new Thickness(0);
                    StandardBorder.Background = Brushes.White;
                    dataWraper.SignetType = "Кругла";
                    dataWraper.SignetSize = Standard.Source.Width + " * " + Standard.Source.Height;
                    
                    //розраховую схожість у відсотках
                    //підганяю розміри шаблона під розмір знайденого зображення
                    var resisedFile = ImageHandler.Encode(template, Result.Source.Width, Result.Source.Height);
                    var diff = recogniser.percentageDiff(recogniser.ResultsPath + "signet.jpg", resisedFile);
                    System.IO.File.Delete(resisedFile);
                    dataWraper.SimilarityDegree = Math.Round(100 - diff, 2) + " %";
                }
                catch (Exception)
                {
                    MessageBox.Show("Помилка при спробі відкриття файлу, перевірте шлях до нього", "Помилка",
                        MessageBoxButton.OK, MessageBoxImage.Error);
                }
            } 
        }
    }
}
