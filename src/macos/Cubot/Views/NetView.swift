import Cubot
import SwiftUI

struct NetView: View {
    @Binding var facesColors: [[cubot.Color?]]

    var body: some View {
        GeometryReader { geometry in
            let faceSize: CGFloat = geometry.size.width / 4.15
            let spacing = 0.05 * faceSize

            VStack(spacing: spacing) {
                HStack(spacing: 5) {
                    Spacer().frame(width: faceSize)
                    FaceView(colors: $facesColors[0])
                        .frame(width: faceSize, height: faceSize)
                    Spacer().frame(width: faceSize)
                    Spacer().frame(width: faceSize)
                }

                HStack(spacing: spacing) {
                    FaceView(colors: $facesColors[3])
                        .frame(width: faceSize, height: faceSize)
                    FaceView(colors: $facesColors[4])
                        .frame(width: faceSize, height: faceSize)
                    FaceView(colors: $facesColors[2])
                        .frame(width: faceSize, height: faceSize)
                    FaceView(colors: $facesColors[5])
                        .frame(width: faceSize, height: faceSize)
                }

                HStack(spacing: spacing) {
                    Spacer().frame(width: faceSize)
                    FaceView(colors: $facesColors[1])
                        .frame(width: faceSize, height: faceSize)
                    Spacer().frame(width: faceSize)
                    Spacer().frame(width: faceSize)
                }
            }
        }
        .aspectRatio(4 / 3, contentMode: .fit)
    }
}
