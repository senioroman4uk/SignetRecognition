using System;
using System.IO;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Drawing;
using System.Drawing.Imaging;

namespace CharacterRecognition
{
    static class ImageHandler
    {
        static public string Encode(string filename, double NewWidth, double NewHeight)
        {
            byte[] array = File.ReadAllBytes(filename);
            Stream stream = new MemoryStream(array);
            BitmapDecoder bdDecoder = BitmapDecoder.Create(stream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.None);
            BitmapFrame bf = bdDecoder.Frames[0];
          
            BitmapFrame bfResize = FastResize(bf, NewWidth, NewHeight);
            byte[] baResize = ToByteArray(bfResize);
            var img = byteArrayToImage(baResize);
            string file = Path.GetFileNameWithoutExtension(filename);
            string newPath = filename.Replace(file, file + "_resised");
            img.Save(newPath, ImageFormat.Jpeg);
            return newPath;
        }

        private static BitmapFrame FastResize(BitmapFrame bfPhoto, double nWidth, double nHeight)
        {
            TransformedBitmap tbBitmap = new TransformedBitmap(bfPhoto, new ScaleTransform(nWidth / bfPhoto.Width, nHeight / bfPhoto.Height, 0, 0));
            return BitmapFrame.Create(tbBitmap);
        }

        public static Image byteArrayToImage(byte[] byteArrayIn)
        {
            MemoryStream ms = new MemoryStream(byteArrayIn);
            Image returnImage = Image.FromStream(ms);
            return returnImage;
        }

        private static byte[] ToByteArray(BitmapFrame bfResize)
        {

            MemoryStream msStream = new MemoryStream();
            JpegBitmapEncoder encoder = new JpegBitmapEncoder();
            encoder.Frames.Add(bfResize);
            encoder.Save(msStream);
            return msStream.ToArray();
        }
    }

}
