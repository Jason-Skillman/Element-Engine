#include "pch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Element {

	static const uint32_t maxFrameBufferSize = 8192;

	namespace Utils {

		inline static GLenum TextureTarget(bool multisample) {
			return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		inline static void CreateTextures(bool multisample, uint32_t* outID, uint32_t count) {
			glCreateTextures(TextureTarget(multisample), count, outID);
		}

		inline static void BindTexture(bool multisample, uint32_t id) {
			glBindTexture(TextureTarget(multisample), id);
		}

		inline static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {
			bool multisample = samples > 1;
			if(multisample)
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisample), id, 0);
		}

		inline static void AttachDepthTexture(uint32_t id, int samples, GLenum attachmentType, GLenum format, uint32_t width, uint32_t height) {
			bool multisample = samples > 1;
			if(multisample)
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			else {
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisample), id, 0);
		}

		static bool IsDepthFormat(FrameBufferTextureFormat format) {
			switch(format) {
				case FrameBufferTextureFormat::DEPTH24STENCIL8: return true;
			}
			return false;
		}

		static GLenum TextureFormatToGLenum(FrameBufferTextureFormat format) {
			switch(format)
			{
				case Element::FrameBufferTextureFormat::RED_INTEGER: 
					return GL_RED_INTEGER;
				case Element::FrameBufferTextureFormat::RGBA8: 
					return GL_RGBA8;
				default:
					EL_CORE_FAIL("Unknown type: {0}", format);
					return 0;
			}
		}
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& specification)
		: rendererID(0), specification(specification) {

		EL_PROFILE_FUNCTION();

		//Get frame buffers from specification
		for(auto spec : specification.attachments.attachments) {
			if(!Utils::IsDepthFormat(spec.textureFormat))
				colorAttachmentSpecifications.emplace_back(spec);
			else
				depthAttachmentSpecification = spec;
		}

		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		EL_PROFILE_FUNCTION();

		glDeleteFramebuffers(1, &rendererID);
		glDeleteTextures(colorAttachmentIDs.size(), colorAttachmentIDs.data());
		glDeleteTextures(1, &depthAttachmentID);
	}

	void OpenGLFrameBuffer::Bind() {
		EL_PROFILE_FUNCTION();
		
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
		glViewport(0, 0, specification.width, specification.height);
	}

	void OpenGLFrameBuffer::Unbind() {
		EL_PROFILE_FUNCTION();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height) {
		if(width == 0 || height == 0 || width > maxFrameBufferSize || height > maxFrameBufferSize) {
			EL_LOG_CORE_ERROR("Invalid frame buffer size x: {0}, y: {1}", width, height);
			return;
		}

		specification.width = width;
		specification.height = height;
		Invalidate();
	}

	int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
		EL_PROFILE_FUNCTION();

		EL_CORE_ASSERT(attachmentIndex < colorAttachmentIDs.size(), "Index is out of range");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
		EL_PROFILE_FUNCTION();

		EL_CORE_ASSERT(attachmentIndex < colorAttachmentIDs.size(), "Index is out of range");

		FrameBufferTextureSpecification& spec = colorAttachmentSpecifications[attachmentIndex];

		glClearTexImage(colorAttachmentIDs[attachmentIndex], 0, Utils::TextureFormatToGLenum(spec.textureFormat), GL_INT, &value);
	}

	void OpenGLFrameBuffer::Invalidate() {
		EL_PROFILE_FUNCTION();

		if(rendererID) {
			glDeleteFramebuffers(1, &rendererID);
			glDeleteTextures(colorAttachmentIDs.size(), colorAttachmentIDs.data());
			glDeleteTextures(1, &depthAttachmentID);

			colorAttachmentIDs.clear();
			depthAttachmentID = 0;
		}
		
		glCreateFramebuffers(1, &rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, rendererID);


		bool multisample = specification.samples > 1;

		//Attachments
		if(colorAttachmentSpecifications.size()) {
			
			//Resize to match size of the specification
			colorAttachmentIDs.resize(colorAttachmentSpecifications.size());

			Utils::CreateTextures(multisample, colorAttachmentIDs.data(), colorAttachmentIDs.size());

			for(size_t i = 0; i < colorAttachmentIDs.size(); i++) {
				Utils::BindTexture(multisample, colorAttachmentIDs[i]);

				switch(colorAttachmentSpecifications[i].textureFormat) {
					case FrameBufferTextureFormat::RED_INTEGER:
						Utils::AttachColorTexture(colorAttachmentIDs[i], specification.samples, GL_R32I, GL_RED_INTEGER, specification.width, specification.height, i);
						break;
					case FrameBufferTextureFormat::RGBA8:
						Utils::AttachColorTexture(colorAttachmentIDs[i], specification.samples, GL_RGBA8, GL_RGBA, specification.width, specification.height, i);
						break;
				}
			}
		}

		//Check if depth needs to be setup
		if(depthAttachmentSpecification.textureFormat != FrameBufferTextureFormat::None) {
			Utils::CreateTextures(multisample, &depthAttachmentID, 1);
			Utils::BindTexture(multisample, depthAttachmentID);

			switch(depthAttachmentSpecification.textureFormat) {
				case FrameBufferTextureFormat::DEPTH24STENCIL8:
					Utils::AttachDepthTexture(depthAttachmentID, specification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, specification.width, specification.height);
					break;
			}
		}

		//
		if(colorAttachmentIDs.size() > 1) {
			EL_CORE_ASSERT(colorAttachmentIDs.size() <= 4, "Too many color attachments! Current limit is 4.");
			GLenum buffer[4]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(colorAttachmentIDs.size(), buffer);
		}
		else if(colorAttachmentIDs.empty()) {
			//If there are no color attachments. Ex. Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		//Attach color buffer
		//glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);
		//glBindTexture(GL_TEXTURE_2D, colorAttachment);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification.width, specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);


		////Attach depth buffer
		//glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachmentID);
		//glBindTexture(GL_TEXTURE_2D, depthAttachmentID);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, specification.width, specification.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachmentID, 0);

		EL_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer is incomplete!");
		
		//Unbind the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
